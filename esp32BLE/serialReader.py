from serial import Serial

BAUD = 115200
PORT = '/dev/cu.usbserial-0001'
PATH = 'scan_data/test_scan.csv'
CSV_HEADER = 'mac_address,time_scanned,rssi,payload'
WRITE_TIME = 10 * 60 # seconds

# Returns time elapsed in seconds
def time_elapsed(data: str) -> float:
    data_split = data.split("\t")
    time_split = data_split[1].split(":")
    # Format HOUR:MIN:SEC:MS
    return (int(time_split[0]) * 3600) + (int(time_split[1]) * 60) + int(time_split[2]) + (float(time_split[3]) / 1000)

# Returns rssi extracted from data.
def rssi(data: str) -> int:
    data_split = data.split("\t")
    return int(data_split[2])
    
def main():
    # Open serial port
    serial_port = Serial(PORT, BAUD)
    print("Listening on port: " + PORT + "...")

    print("Writing to " + PATH + "...")
    output_file = open(PATH, 'w')
    output_file.write(CSV_HEADER + "\n")
    output_file.flush()

    # Read data from serial port and write to the file
    while True:
        if serial_port.in_waiting > 0:
            data = serial_port.readline().decode()
            
            if ((len(data) > 0) and (data[0] == "~")):
                if (time_elapsed(data) > WRITE_TIME):
                    output_file.close()
                    break

                if (rssi(data) >= -35):
                    print(data)
                    data = data.replace("~", "").replace("\t", ",")
                    output_file.write(data)
                    output_file.flush()
            else:
                print(data)

if __name__ == "__main__":
    main()
    