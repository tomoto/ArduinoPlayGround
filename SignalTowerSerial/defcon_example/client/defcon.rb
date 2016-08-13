require "redis"
require "serialport"

COMM_SPEED = 9600

DEFCON = {
  1 => '{"all":"blink"}',
  2 => '{"all":"off", "blue":"blink", "green":"blink", "yellow":"blink"}',
  3 => '{"all":"off", "blue":"blink", "green":"blink"}',
  4 => '{"all":"off", "blue":"blink"}',
  5 => '{"all":"off"}'
}

serial_port = ARGV[0]

redis = Redis.new

loop do
  begin
    puts "Opening serial port " + serial_port
    SerialPort.open(serial_port, COMM_SPEED, 8, 1, SerialPort::NONE) do |serial|
      sleep 3
      puts "Ready"
      loop do
        (key, defcon_level) = redis.blpop("defcon", 0)
        puts "Received "  + defcon_level
        json = DEFCON[defcon_level.to_i]
        puts "Sending " + json
        serial.write json + "\n"
      end
    end
  rescue Exception => e
    p e
    sleep 5
  end
end
