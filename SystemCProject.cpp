#include <systemc.h>

bool flagTem = true;
bool flagHum = true;
bool flagPressure = true;

// Khối Controller
SC_MODULE(Controller) {
    sc_in<double> desiredTemp; // Tín hiệu nhiệt độ mong muốn
    sc_in<double> desiredHumidity; // Tín hiệu độ ẩm mong muốn
    sc_in<double> desiredPressure; // Tín hiệu áp suất mong muốn
    sc_in<double> currentPressure;
    sc_in<double> currentTemp;
    sc_in<double> currentHumidity;
    sc_out<bool> fan; // Tín hiệu bật/tắt quạt
    sc_out<bool> pump; // Tín hiệu bật/tắt bơm
    sc_out<bool> heater; // Tín hiệu bật/tắt bộ sưởi
    sc_out<bool> compressor; // Tín hiệu bật/tắt bộ làm lạnh

    void controlProcess() {
        while (true) {
            if (currentTemp.read() < desiredTemp.read() || currentTemp.read() == 20) {
                compressor.write(false); // Tắt bộ làm lạnh
                heater.write(true); // Bật sưởi
                flagTem = true;
            }
            if (currentTemp.read() > desiredTemp.read() || currentTemp.read() == 32) {
                compressor.write(true); // Bật bộ làm lạnh
                heater.write(false); // Tắt sưởi
                flagTem = false;
            }

            if (currentHumidity.read() <= desiredHumidity.read() && currentPressure.read() >= desiredPressure.read()) {
                fan.write(false); // Tắt quạt
                pump.write(false); // Tắt bơm
                flagHum = false;
                flagPressure = false;
            }
            else {
                fan.write(true); // Bật quạt
                pump.write(true); // Bật bơm
                flagHum = true;
                flagPressure = true;
            }

            wait(2, SC_SEC); // Đợi 4 giây trước khi kiểm tra lại
        }
    }

    SC_CTOR(Controller) {
        SC_THREAD(controlProcess);
        sensitive << currentTemp << desiredTemp << currentHumidity << desiredHumidity << currentPressure << desiredPressure;
    }
};

// Khối Software
SC_MODULE(Software) {
    sc_in<bool> fan; // Tín hiệu bật/tắt quạt
    sc_in<bool> pump; // Tín hiệu bật/tắt bơm
    sc_in<bool> heater; // Tín hiệu bật/tắt bộ lọc nhiệt
    sc_in<bool> compressor; // Tín hiệu bật/tắt bộ làm lạnh 
    sc_in<double> currentTemp; // Tín hiệu nhiệt độ hiện tại
    sc_in<double> currentHumidity; // Tín hiệu độ ẩm hiện tại
    sc_in<double> currentPressure; // Tín hiệu áp suất hiện tại

    void softwareProcess() {
        while (true) {
            double temp = currentTemp.read();
            double hum = currentHumidity.read();
            double pres = currentPressure.read();
            bool fanStatus = fan.read();
            bool pumpStatus = pump.read();
            bool heaterStatus = heater.read();
            bool compressorStatus = compressor.read();

            std::cout << sc_time_stamp() << endl;
            std::cout << "\t Current Temperature: " << temp << " C" << endl;
            std::cout << "\t Current Humidity: " << hum << " %" << endl;
            std::cout << "\t Current Pressure: " << pres << " hPa" << endl;
            std::cout << "\t Fan Status: " << (fanStatus ? "ON" : "OFF") << endl;
            std::cout << "\t Pump Status: " << (pumpStatus ? "ON" : "OFF") << endl;
            std::cout << "\t Heater Status: " << (heaterStatus ? "ON" : "OFF") << endl;
            std::cout << "\t Compressor Status: " << (compressorStatus ? "ON" : "OFF") << endl;
            cout << "=========================" << endl;

            wait(3, SC_SEC); // Đợi 3 giây trước khi kiểm tra lại
        }
    }

    SC_CTOR(Software) {
        SC_THREAD(softwareProcess);
        sensitive << currentTemp << currentHumidity << currentPressure << fan << pump << heater << compressor;
    }
};

// Khối InputControl
SC_MODULE(InputControl) {
    sc_out<double> desiredTemp; // Tín hiệu nhiệt độ mong muốn
    sc_out<double> desiredHumidity; // Tín hiệu độ ẩm mong muốn
    sc_out<double> desiredPressure; // Tín hiệu áp suất mong muốn

    void inputProcess() {
        double temp; // Start with cooling temperature
        double humidity = 40.0;
        double pressure = 1013;

        while (true) {
            // Check if the system is in cooling or heating mode
            if (flagTem == true) {
                temp = 32;
            }
            else {
                temp = 20;
            }

            // Rest of the code remains unchanged
            desiredTemp.write(temp);
            desiredHumidity.write(humidity);
            desiredPressure.write(pressure);

            wait(1, SC_SEC);
        }
    }

    SC_CTOR(InputControl) {
        SC_THREAD(inputProcess);
    }
};

// Khối DHT11
SC_MODULE(DHT11) {
    sc_out<double> currentTemp; // Tín hiệu nhiệt độ hiện tại
    sc_out<double> currentHumidity; // Tín hiệu độ ẩm hiện tại

    void readTemperature() {
        double temp = 27;
        double hum = 60;

        while (true) {
            if (flagHum == true) {
                hum -= 1;
            }
                
        if (flagTem == true) {
            temp += 0.5;
        }
        else {
            temp -= 0.5;
        }
        
        currentTemp.write(temp);
        currentHumidity.write(hum);
        wait(3, SC_SEC);
        }
    }

    SC_CTOR(DHT11) {
        SC_THREAD(readTemperature);
    }
};

// Khối BMP180
SC_MODULE(BMP180) {
    sc_out<double> currentPressure;

    void readPressure() {
        double pressure = 1000;

        while (true) {
            if (flagPressure == true) {
                pressure += 0.7;
            }
            currentPressure.write(pressure);
            wait(3, SC_SEC);
        }
    }

    SC_CTOR(BMP180) {
        SC_THREAD(readPressure);
    }
};

int sc_main(int argc, char* argv[]) {
    // Tạo tín hiệu và đối tượng
    sc_signal<double> desiredTemp;
    sc_signal<double> desiredHumidity;
    sc_signal<double> desiredPressure;
    sc_signal<bool> compressor;
    sc_signal<bool> fan;
    sc_signal<bool> pump;
    sc_signal<bool> heater;
    sc_signal<double> currentTemp;
    sc_signal<double> currentHumidity;
    sc_signal<double> currentPressure;

    Controller controller("Controller");
    controller.desiredTemp(desiredTemp);
    controller.desiredHumidity(desiredHumidity);
    controller.desiredPressure(desiredPressure);
    controller.currentTemp(currentTemp);
    controller.currentHumidity(currentHumidity);
    controller.currentPressure(currentPressure);
    controller.pump(pump);
    controller.fan(fan);
    controller.heater(heater);
    controller.compressor(compressor);

    Software software("Software");
    software.fan(fan);
    software.pump(pump);
    software.heater(heater);
    software.compressor(compressor);
    software.currentTemp(currentTemp);
    software.currentHumidity(currentHumidity);
    software.currentPressure(currentPressure);

    InputControl inputControl("InputControl");
    inputControl.desiredTemp(desiredTemp);
    inputControl.desiredHumidity(desiredHumidity);
    inputControl.desiredPressure(desiredPressure);

    DHT11 dht11("DHT11");
    dht11.currentTemp(currentTemp);
    dht11.currentHumidity(currentHumidity);

    BMP180 bmp180("BMP180");
    bmp180.currentPressure(currentPressure);

    // Chạy mô phỏng
    sc_start(120, SC_SEC);

    return 0;
}