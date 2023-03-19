/**
   * genpw: Simple password generator
   */

#include <cassert>
#include <climits>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using std::string;

// strong random int generator
static int rand_int(int max = INT_MAX - 1)
{
   assert(max < INT_MAX);

   static std::random_device                 rd;
   static std::mt19937_64                    generator(rd());
   static std::uniform_int_distribution<int> random_number(1, INT_MAX);
   return random_number(generator) % max;
}

void print_help()
{
   std::cout << "Help for genpw (by default it generates passwords with 8 lowercase letters)\n"
             << "\t-h:\t display this help\n"
             << "\t-len n:\t specify the length\n"
             << "\t-l:\t include lowercase\n"
             << "\t-u:\t include uppercase\n"
             << "\t-n:\t include numbers\n"
             << "\t-p:\t include punctuation" << std::endl;
}

enum Options
{
   Lowercase   = 1 << 0,
   Uppercase   = 1 << 1,
   Numbers     = 1 << 2,
   Punctuation = 1 << 3,
};
using PasswordGeneratorOptions = int; // Holds the options above

struct PasswordGenerator
{
   // Options
   PasswordGeneratorOptions opts;
   // Length of the password
   int len;
   // Holds the characters that the generator will use
   std::vector<char> characters{};

   // Initilize the password generator
   PasswordGenerator(const PasswordGeneratorOptions options, const int length);
   // Generate the password
   // if length is <= 0, the len class member is used
   string generate(int length = -1) const;
};

PasswordGenerator::PasswordGenerator(const PasswordGeneratorOptions options, const int length)
{
   len = length;

   if (options & Lowercase)
      for (char c = 'a'; c <= 'z'; ++c)
         characters.push_back(c);
   if (options & Uppercase)
      for (char c = 'A'; c <= 'Z'; ++c)
         characters.push_back(c);
   if (options & Numbers)
      for (char c = '0'; c <= '9'; ++c)
         characters.push_back(c);
   if (options & Punctuation)
      for (char c : "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~")
         characters.push_back(c);
}

string PasswordGenerator::generate(int length) const
{
   if (length <= 0)
      length = len;
   string passwd{};
   std::cout << characters.size() << std::endl;
   for (int i = 0; i < length; ++i)
   {
      passwd += characters[rand_int(characters.size())];
   }
   return passwd;
}

PasswordGenerator parse_args(int argc, char* argv[])
{
   PasswordGeneratorOptions opts = 0;
   int                      len  = 8;

   for (int i = 1; i < argc; ++i)
   {
      // C strings btfo
      string arg = argv[i];
      if (arg == "-l")
         opts |= Lowercase;
      else if (arg == "-u")
         opts |= Uppercase;
      else if (arg == "-n")
         opts |= Numbers;
      else if (arg == "-p")
         opts |= Punctuation;
      // Specify length
      else if (arg == "-len")
      {
         string len_str = (argv[++i]);
         try
         {
            len = std::stoi(len_str);
            if (len <= 0) // no skidding
            {
               std::cerr << "Retard, get yourself together.\n";
               exit(1);
            }
         }
         catch (std::invalid_argument& e) // if couldn't convert to int
         {
            std::cerr << "Error: invalid password length, exiting...\n";
            exit(1);
         }
      }
      else if (arg == "-h")
      {
         print_help();
         exit(0);
      }
      else
      {
         std::cerr << "Unrecognized argument: " << arg << std::endl;
         exit(1);
      }
   }

   if (opts == 0)
      opts = Lowercase;

   return { opts, len };
}

int main(int argc, char* argv[])
{
   PasswordGenerator pwgen = parse_args(argc, argv);
   std::cout << pwgen.generate() << '\n';
}
