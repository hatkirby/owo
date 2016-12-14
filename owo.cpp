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
  
  verbly::data database {config["verbly_datafile"].as<std::string>()};

  verbly::noun bp = database.nouns().with_wnid(105220461).run().front(); // body part
  verbly::noun pp = database.nouns().with_wnid(104723816).run().front(); // quality
  verbly::noun cp = database.nouns().with_wnid(103051540).run().front(); // clothing
  verbly::filter<verbly::noun> filt {bp, pp, cp};
  filt.set_orlogic(true);
  
  for (;;)
  {
    std::cout << "Generating tweet" << std::endl;
    
    auto ns = database.nouns().full_hyponym_of(filt).is_not_proper().random().limit(1).run();
    verbly::noun n = ns.front();
    
    std::string result = "*notices ur " + n.base_form() + "* OwO whats this..?";
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
