require "serialport"

COMM_SPEED = 9600 * 12

serial_port = ARGV[0]

SAMPLE_DURATION = 10 # milliseconds
timestamp_base = 0
samples = 0
total_current = 0
max_current = 0
min_current = 0

SerialPort.open(serial_port, COMM_SPEED, 8, 1, SerialPort::NONE) do |serial|
  loop do
    while (serial.readchar() != '$') do
      # wait for the header
    end
    
    (timestamp, current) = serial.read(8).unpack("L<e")
    
    if (timestamp - timestamp_base > SAMPLE_DURATION || timestamp < timestamp_base)
      if (samples > 0)
        print (total_current / samples).to_s + "," +
          min_current.to_s + "," +
          max_current.to_s + "\n";
      end
      
      timestamp_base = (timestamp / SAMPLE_DURATION).to_i * SAMPLE_DURATION
      samples = 0
      total_current = 0
      max_current = -999999
      min_current = 999999
    end
    
    samples += 1
    total_current += current;
    max_current = [current, max_current].max
    min_current = [current, min_current].min
  end
end
