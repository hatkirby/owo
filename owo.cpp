#include <yaml-cpp/yaml.h>
#include <iostream>
#include <sstream>
#include <twitter.h>
#include <verbly.h>
#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cout << "usage: owo [configfile]" << std::endl;
    return -1;
  }

  std::string configfile(argv[1]);
  YAML::Node config = YAML::LoadFile(configfile);

  twitter::auth auth;
  auth.setConsumerKey(config["consumer_key"].as<std::string>());
  auth.setConsumerSecret(config["consumer_secret"].as<std::string>());
  auth.setAccessKey(config["access_key"].as<std::string>());
  auth.setAccessSecret(config["access_secret"].as<std::string>());

  twitter::client client(auth);

  verbly::database database {config["verbly_datafile"].as<std::string>()};

  verbly::filter nounFilter =
    (verbly::notion::partOfSpeech == verbly::part_of_speech::noun)
    && (verbly::form::proper == false)
    && (verbly::notion::fullHypernyms %= (
      (verbly::notion::wnid == 105220461)  // body part
      || (verbly::notion::wnid == 104723816) // quality
      || (verbly::notion::wnid == 103051540))); // clothing

  for (;;)
  {
    std::cout << "Generating tweet" << std::endl;

    verbly::word noun = database.words(nounFilter).first();
    verbly::token action = {
      verbly::token::quote("*", "*", { "notices ur", noun }),
      "OwO whats this..?"
    };

    std::string result = action.compile();
    result.resize(140);

    try
    {
      client.updateStatus(result);
    } catch (const twitter::twitter_error& e)
    {
      std::cout << "Twitter error: " << e.what() << std::endl;
    }

    std::cout << result << std::endl;

    std::this_thread::sleep_for(std::chrono::hours(1));
  }
}
