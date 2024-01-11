import time
import array
import math
import board
import busio
import digitalio
import audiobusio
import json
import adafruit_apds9960.apds9960
import adafruit_bmp280
import adafruit_lis3mdl
import adafruit_lsm6ds.lsm6ds33
import adafruit_sht31d
import adafruit_rfm9x
import adafruit_datetime
import rtc

# Initialize I2C bus and sensors
i2c = board.I2C()
apds9960 = adafruit_apds9960.apds9960.APDS9960(i2c)
bmp280 = adafruit_bmp280.Adafruit_BMP280_I2C(i2c)
lis3mdl = adafruit_lis3mdl.LIS3MDL(i2c)
lsm6ds33 = adafruit_lsm6ds.lsm6ds33.LSM6DS33(i2c)
sht31d = adafruit_sht31d.SHT31D(i2c)
microphone = audiobusio.PDMIn(board.MICROPHONE_CLOCK, board.MICROPHONE_DATA, sample_rate=16000, bit_depth=16)

apds9960.enable_color = True

# Function to compute sound level
def normalized_rms(values):
    minbuf = int(sum(values) / len(values))
    return int(math.sqrt(sum(float(sample - minbuf) * (sample - minbuf) for sample in values) / len(values)))

# LoRa configuration
CS = digitalio.DigitalInOut(board.D11)
RESET = digitalio.DigitalInOut(board.D10)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, 434.0)
rfm9x.tx_power = 13
rfm9x.signal_bandwidth = 125000
rfm9x.coding_rate = 5
rfm9x.spreading_factor = 7
rfm9x.enable_crc = True

while True:
    # Read sensor data
    samples = array.array('H', [0] * 160)
    microphone.record(samples, len(samples))
    sound_level = normalized_rms(samples)

    # Prepare data packet
    data_packet = {
        "pointId": 1,
        "humidity": sht31d.relative_humidity,
        "pressure": bmp280.pressure,
        "temperature" : bmp280.temperature,
        "altitude": bmp280.altitude,
        "magnetometer": lis3mdl.magnetic[0], 
        "sound": sound_level,
        "colour": "Red: {}, Green: {}, Blue: {}, Clear: {}".format(*apds9960.color_data)
    }

    # Convert dictionary to JSON string
    json_data = json.dumps(data_packet)

    print(json_data)

    # Send data over LoRa
    rfm9x.send(bytes(json_data, "UTF-8"))

    # Wait for a bit before next reading
    time.sleep(30)
