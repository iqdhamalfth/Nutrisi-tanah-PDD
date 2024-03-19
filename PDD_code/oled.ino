void oledShow() {
  display.clearDisplay();  // clear display        // set text size
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 5);
  display.print("Humidity: ");
  display.print(dataSoil);
  display.println(" %");
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 15);
  display.print("Temperature: ");
  display.print(dataTemp);
  display.println(" C");
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 35);
  display.print("N: ");
  display.print(dataNitrogen);
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(45, 35);
  display.print("P: ");
  display.print(dataPhosphorus);
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(90, 35);
  display.print("K: ");
  display.print(dataPotassium);
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(15, 55);
  display.print("EC: ");
  display.print(dataConduct);
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(75, 55);
  display.print("pH: ");
  display.print(datapH);
  display.display();
  delay(2000);

  display.clearDisplay();  // clear display        // set text size
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("Location: "));
  display.setCursor(0, 10);
  display.print(gps.location.lat(), 6);
  display.print(F(","));
  display.print(gps.location.lng(), 6);
  display.display();

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print("Date: ");
  display.print(gps.date.month());
  display.print(F("/"));
  display.print(gps.date.day());
  display.print(F("/"));
  display.print(gps.date.year());
  display.display();

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.print(weekDays[timeClient.getDay()]);
  display.print(", ");
  display.print(timeClient.getHours());
  display.print(":");
  display.print(timeClient.getMinutes());
  display.print(":");
  display.println(timeClient.getSeconds());
  display.display();
  delay(2000);
}