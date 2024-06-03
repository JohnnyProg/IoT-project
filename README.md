# Projekt z przedmiotu Internet Rzeczy (IoT)

# Opis 
Projekt ten dotyczy stworzenia urządzenia IoT z wykorzystaniem ESP32 oraz platformy RainMaker, które mierzy temperaturę, wilgotność i ciśnienie atmosferyczne za pomocą czujnika BME280 oraz steruje diodą RGB. Wszystko jest zintegrowane z platformą ESP RainMaker, co pozwala na zdalne monitorowanie i sterowanie za pomocą aplikacji mobilnej oraz Asystenta Google (GVA).

Funkcje Projektu
- Pomiar Warunków Środowiskowych:
  Czujnik BME280 mierzy temperaturę, wilgotność i ciśnienie atmosferyczne.
  Dane są przesyłane do chmury ESP RainMaker.
- Sterowanie Diodą RGB:
  Dioda RGB jest kontrolowana przez aplikację mobilną RainMaker i Asystenta Google.
  Możliwość zmiany koloru, jasności, odcienia, nasycenia i intensywności światła.
- Łączność i Provisioning:
  Użycie Bluetooth (BLE) do początkowego provisioningu Wi-Fi oraz sparowania urządzenia z kontem użytkownika.
  Urządzenie łączy się z siecią Wi-Fi i przesyła dane z czujników.
- Resetowanie:
  Możliwość resetowania urządzenia do ustawień fabrycznych lub tylko Wi-Fi za pomocą przycisku.

# Schemat
<img src="https://github.com/JohnnyProg/IoT-project/blob/main/esp32_schema.png" />

