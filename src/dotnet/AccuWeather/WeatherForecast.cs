namespace AccuWeather;

// Модель данных
public class WeatherForecast
{
    public int Id { get; set; }
    public DateTime DateTime { get; set; } // Время прогноза
    public DateTime CreatedAt { get; set; } // Время сохранения
    public double Temperature { get; set; }
    public string Description { get; set; }
    public int PrecipitationProbability { get; set; }
    public int WindSpeed { get; set; }
    public int WeatherIcon { get; set; }
    
}