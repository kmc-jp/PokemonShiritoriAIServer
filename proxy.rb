# coding: utf-8
require 'json'
require 'securerandom'
require 'open3'
require 'sinatra/base'

def load_dict
  file = open('pokemon_shiritori_dict.json')
  return JSON.parse(file.read)
end

$dict = load_dict
def get_index(chr)
  kana = "あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわ"
  return kana.index(chr)
end

class Game
  def initialize
    @unused = $dict.dup
    @table = Array.new(44)
    44.times do |i|
      @table[i] = Array.new(44, 0)
    end
    @unused.each do |h|
      next if h["kana"][-1] == "ん"
      k = h["kana"]
      @table[get_index(k[0])][get_index(k[-1])] += 1
    end
    @pos = nil
  end
  def use(str)
    h = @unused.find {|h| h["name"] == str}
    k = h["kana"]
    @table[get_index(k[0])][get_index(k[-1])] -= 1
    @unused.delete(h)
    @pos = get_index(k[-1])
  end
  def call_ai
    res = 0
    Open3.popen2("./a.out") {|stdin, stdout, wait_thr|
      stdin.puts @pos
      @table.each {|l|
        stdin.puts l.join(' ')
      }
      stdin.close
      res = stdout.read.to_i
    }
    return res
  end
  def play(str)
    use(str)
    res = call_ai
    bot_h = @unused.find {|h|
      next if h["kana"][-1] == "ん"
      k = h["kana"]
      @pos == get_index(k[0]) and res == get_index(k[-1])
    }
    use(bot_h["name"])
    return bot_h["name"]
  end
end

$games = Hash.new
class ShiritoriServer < Sinatra::Base
  get '/' do
    'start game: /start/, play game: /play/:uuid/:word'
  end
  get '/start/' do
    u = SecureRandom.uuid
    $games[u] = Game.new
    return u
  end
  get '/play/:uuid/:word' do
    u = params[:uuid]
    if $games.key?(u) then
      puts "player: #{params[:word]}"
      ret = $games[u].play(params[:word])
      puts "bot: #{ret}"
      return ret
    end
  end
end

ShiritoriServer.run!
