using System.Text.Json;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;

namespace AccuWeather;

// Фоновый сервис для обновления данных
public class WeatherUpdater : BackgroundService
{
    private readonly IHttpClientFactory _clientFactory;
    private readonly IServiceProvider _services;
    private readonly string _apiKey;
    private readonly string _locationKey;
    private readonly ILogger<WeatherUpdater> _logger;

    public WeatherUpdater(
        IHttpClientFactory clientFactory,
        IServiceProvider services,
        IConfiguration config,
        ILogger<WeatherUpdater> logger)
    {
        _clientFactory = clientFactory;
        _services = services;
        _apiKey = config["AccuWeather:ApiKey"];
        _locationKey = config["AccuWeather:LocationKey"];
        _logger = logger;
    }

    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        // Ждем первое обновление
        //await UpdateWeatherData();

        while (!stoppingToken.IsCancellationRequested)
        {
            using (var scope = _services.CreateScope())
            {
                var db = scope.ServiceProvider.GetRequiredService<WeatherDbContext>();
                
                // Получаем время последнего успешного обновления
                var lastRun = await db.AppStates
                    .FirstOrDefaultAsync(a => a.Id == "LastRunTime");

                var nextRun = lastRun?.Value.AddMinutes(30) ?? DateTime.UtcNow;
                var delay = nextRun - DateTime.UtcNow;

                // Если задержка отрицательная - запускаем сразу
                if (delay > TimeSpan.Zero)
                {
                    _logger.LogInformation($"Next update in {delay.TotalMinutes:N1} minutes");
                    await Task.Delay(delay, stoppingToken);
                }
            }

            await UpdateWeatherData();
        }
    }

    private async Task UpdateWeatherData()
    {
        try
        {
            var client = _clientFactory.CreateClient();
            var url = $"http://dataservice.accuweather.com/forecasts/v1/hourly/12hour/{_locationKey}?" +
                      $"apikey={_apiKey}&language=ru-ru&metric=true&details=true";
            var response = await client.GetAsync(url);
                         
            response.EnsureSuccessStatusCode();

            var content = await response.Content.ReadAsStringAsync();
            var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
            var forecasts = JsonSerializer.Deserialize<List<AccuWeatherResponse>>(content, options);

            using var scope = _services.CreateScope();
            var db = scope.ServiceProvider.GetRequiredService<WeatherDbContext>();

            // Обновляем прогнозы
            db.Forecasts.RemoveRange(db.Forecasts);
            
            var entities = forecasts.Select(f => new WeatherForecast
            {
                DateTime = f.DateTime.ToUniversalTime(),
                CreatedAt = DateTime.UtcNow,
                Temperature = f.Temperature.Value,
                Description = f.IconPhrase,
                PrecipitationProbability = f.PrecipitationProbability,
                WindSpeed = (int)Math.Round((f.Wind?.Speed?.Value ?? 0) * (1/3.6)),
                WeatherIcon = f.WeatherIcon,
            });
            await db.Forecasts.AddRangeAsync(entities);

            // Обновляем время последнего запуска
            var lastRunState = await db.AppStates
                .FirstOrDefaultAsync(a => a.Id == "LastRunTime");
            
            if (lastRunState == null)
            {
                lastRunState = new ApplicationState { Id = "LastRunTime" };
                await db.AppStates.AddAsync(lastRunState);
            }

            lastRunState.Value = DateTime.UtcNow;
            await db.SaveChangesAsync();

            _logger.LogInformation("Weather data updated successfully at {Time}", DateTime.UtcNow);
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Error updating weather data");
        }
    }
}