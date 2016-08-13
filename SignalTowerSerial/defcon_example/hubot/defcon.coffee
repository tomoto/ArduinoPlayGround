# Description:
#   Posts the signal tower control event to redis responding to defcon message
#
# Commands:
#   hubot defcon <1-5>
#

Redis = require "redis"

module.exports = (robot) =>

  robot.respond /defcon (\d+)/i, (res) ->
    level = +res.match[1]
    if (level >= 1 && level <= 5)
      client = Redis.createClient();
      client.rpush("defcon", level);
      client.expire("defcon", 60*15);
      client.quit();
      
      res.send "Declared Defcon " + level + "!"
    else
      res.reply "Defcon level should be from 1 (danger) to 5 (peace)."
