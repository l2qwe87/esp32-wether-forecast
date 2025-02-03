using Microsoft.EntityFrameworkCore;

namespace AccuWeather;

// Контекст БД
public class WeatherDbContext : DbContext
{
    public DbSet<WeatherForecast> Forecasts { get; set; }
    public DbSet<ApplicationState> AppStates { get; set; }

    public WeatherDbContext(DbContextOptions<WeatherDbContext> options) : base(options) { }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<WeatherForecast>(entity =>
        {
            entity.HasIndex(f => f.DateTime);
            entity.Property(f => f.Description).HasMaxLength(100);
        });
        
        modelBuilder.Entity<ApplicationState>(entity => 
        {
            entity.HasKey(a => a.Id);
            entity.Property(a => a.Id).HasMaxLength(100);
        });
    }
}