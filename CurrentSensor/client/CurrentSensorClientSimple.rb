require "serialport"

COMM_SPEED = 9600 * 12

serial_port = ARGV[0]

SerialPort.open(serial_port, COMM_SPEED, 8, 1, SerialPort::NONE) do |serial|
  loop do
    while (serial.readchar() != '$') do
      # wait for the header
    end
    
    (timestamp, current) = serial.read(8).unpack("L<e")
    
    print timestamp.to_s + "," + current.to_s + "\n"
  end
end
