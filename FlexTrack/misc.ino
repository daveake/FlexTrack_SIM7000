
double CalculateDistance(double Latitude1, double Longitude1, double Latitude2, double Longitude2)
{
  Latitude1 = Latitude1 * PI / 180;
  Longitude1 = Longitude1 * PI / 180;
  Latitude2 = Latitude2 * PI / 180;
  Longitude2 = Longitude2 * PI / 180;

  return 6371000.0 * acos(sin(Latitude2) * sin(Latitude1) + cos(Latitude2) * cos(Latitude1) * cos(Longitude2-Longitude1));
}
