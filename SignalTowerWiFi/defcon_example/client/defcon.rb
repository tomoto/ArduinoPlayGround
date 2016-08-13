require "redis"
require "net/http"

COMM_SPEED = 9600

DEFCON = {
  1 => '{"all":"blink"}',
  2 => '{"all":"off", "blue":"blink", "green":"blink", "yellow":"blink"}',
  3 => '{"all":"off", "blue":"blink", "green":"blink"}',
  4 => '{"all":"off", "blue":"blink"}',
  5 => '{"all":"off"}'
}

redis = Redis.new

loop do
  begin
    (key, defcon_level) = redis.blpop("defcon", 0)
    puts "Received "  + defcon_level
    uri = URI("http://signaltower/states")
    uri.query = URI.encode_www_form(:json => DEFCON[defcon_level.to_i])
    puts Net::HTTP.get(uri)
  rescue Exception => e
    p e
    sleep 5
  end
end
