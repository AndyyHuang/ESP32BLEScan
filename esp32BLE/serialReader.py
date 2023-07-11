from serial import Serial
import os

file_path = 'scan_data/iPhone14proTapped.csv'

# Open the serial port
serial_port = Serial('/dev/cu.usbserial-0001', 115200)

print("Listening on port: " + '/dev/cu.usbserial-0001' + "...")
print("Writing to " + file_path)
# Open the file for writing
output_file = open(file_path, 'w')
file_size = os.path.getsize(file_path)
if file_size == 0:
    output_file.write("mac_address,time_scanned,rssi,payload\n")
    output_file.flush()

# Read data from serial port and write to the file
while True:
    if serial_port.in_waiting > 0:
        data = serial_port.readline().decode()
        print(data)
        if (len(data) > 0) and data[0] == "~":
            data = data.replace("~", "").replace("\t", ",")
            output_file.write(data)
            output_file.flush()