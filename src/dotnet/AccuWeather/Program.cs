using Microsoft.AspNetCore.Builder;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Configuration;
using AccuWeather;
using Microsoft.AspNetCore.Http;

var builder = WebApplication.CreateBuilder(args);

// Конфигурация
builder.Configuration.AddJsonFile("appsettings.json");

// База данных
builder.Services.AddDbContext<WeatherDbContext>(options =>
    options.UseSqlite("Data Source=weather.db"));

// Фоновый сервис
builder.Services.AddHostedService<WeatherUpdater>();

// HTTP-клиент
builder.Services.AddHttpClient();

var app = builder.Build();

// API Endpoint
app.MapGet("/forecast", async (WeatherDbContext db, int offsetHours = 0) =>
{
    var targetTime = DateTime.UtcNow.AddHours(offsetHours);
    
    var forecast = db.Forecasts
        .Where(f => f.DateTime >= targetTime.AddHours(-1) && f.DateTime <= targetTime.AddHours(1))
        .AsEnumerable()
        .OrderBy(f => Math.Abs((f.DateTime - targetTime).Ticks))
        .FirstOrDefault();

    var moscowTime = forecast.DateTime;
    moscowTime = moscowTime.AddHours(3);
    return forecast != null ? Results.Ok(new
    {
        forecast.DateTime,
        TimeMoscow = moscowTime.ToString("HH:mm"),
        forecast.Temperature,
        forecast.Description,
        forecast.PrecipitationProbability,
        forecast.WeatherIcon,
        forecast.WindSpeed,
    }) : Results.NotFound();
});

// Создание БД при первом запуске
using (var scope = app.Services.CreateScope())
{
    var db = scope.ServiceProvider.GetRequiredService<WeatherDbContext>();
    db.Database.EnsureCreated();
}

app.Run();