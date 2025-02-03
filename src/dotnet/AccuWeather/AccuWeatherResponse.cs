using System.Text.Json.Serialization;

namespace AccuWeather;

public class AccuWeatherResponse
{
    [JsonPropertyName("DateTime")]
    public DateTime DateTime { get; set; }

    [JsonPropertyName("WeatherIcon")]
    public int WeatherIcon { get; set; }

    [JsonPropertyName("IconPhrase")]
    public string IconPhrase { get; set; }

    [JsonPropertyName("Temperature")]
    public AccuTemperature Temperature { get; set; }

    [JsonPropertyName("PrecipitationProbability")]
    public int PrecipitationProbability { get; set; }

    [JsonPropertyName("Wind")]
    public AccuWind Wind { get; set; }
}

public class AccuTemperature
{
    [JsonPropertyName("Value")]
    public double Value { get; set; }

    [JsonPropertyName("Unit")]
    public string Unit { get; set; }
}

public class AccuWind
{
    [JsonPropertyName("Speed")]
    public AccuWindSpeed Speed { get; set; }
}

public class AccuWindSpeed
{
    [JsonPropertyName("Value")]
    public double Value { get; set; }

    [JsonPropertyName("Unit")]
    public string Unit { get; set; }
}