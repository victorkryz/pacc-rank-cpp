#include <memory>
#include <cstring>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <exception>
#include <filesystem>
#include <queue>
#include <functional>
#include <map>
#include <unordered_map>
#include <algorithm>

namespace fs = std::filesystem;

struct rank_entry_t
{
   std::uint32_t candidate_id;
   std::uint32_t rank;

   bool operator<(const rank_entry_t& other) const {
      return rank < other.rank;
   }
};

using ranks_t=std::unordered_map<std::uint32_t, std::uint32_t>;
using sorted_ranks_t=std::vector<rank_entry_t>; 

std::ifstream get_protocol();
ranks_t parse_protocol(std::ifstream& fs);
sorted_ranks_t sort_ranks(const ranks_t& ranks);
void print_ranks(const sorted_ranks_t& ranks);
 
int main(int argc, char* argv[])
{
   int result(0);

   try
   {
      std::cout << fs::current_path() << std::endl;

      std::ifstream fs = get_protocol();
      auto ranks = parse_protocol(fs);
      fs.close();

      sorted_ranks_t sranks = sort_ranks(ranks);
      print_ranks(sranks);

   }
   catch(const std::exception& e)
   {
      std::cerr << e.what() << '\n';
   }
   
   return result;
}	


std::ifstream get_protocol()
{
   std::ifstream file("data/protocol.txt");
   if ( !file)
   {
      throw std::runtime_error("Cannot open protocol file");
   }
   return file;
}

void extract_rank_values(const std::string& rank_sequence, ranks_t& ranks)
{
   static const char RANK_VALUES_DELIMITER = ',';

   std::stringstream ss(rank_sequence);
   std::string s_value;
   while ( std::getline(ss, s_value, RANK_VALUES_DELIMITER) )
   {
         const int val = std::stoi(s_value);
         ranks_t::value_type::second_type& count = ranks[val];
         count++;
   }
}

ranks_t parse_protocol(std::ifstream& fs)
{
   static const std::string RANK_PREFIX = "V=";
   static const char RANK_END_DELIMITER = ' ';

   ranks_t ranks;
   for (std::string line; std::getline(fs, line);)
   {  
      size_t rank_pos = line.find(RANK_PREFIX);
      if (rank_pos != std::string::npos) 
      {
         rank_pos+=std::size(RANK_PREFIX);

         const size_t rank_end_pos = line.find_first_of(RANK_END_DELIMITER, rank_pos);
         const std::string rank_sequence = line.substr(rank_pos, rank_end_pos);

         extract_rank_values(rank_sequence, ranks);
      }
   }
   
   return ranks;
}

sorted_ranks_t sort_ranks(const ranks_t& ranks)
{
   sorted_ranks_t sranks;
   sranks.resize(ranks.size());

   std::transform(ranks.begin(), ranks.end(),
                  sranks.begin(), [](const ranks_t::value_type& v) -> rank_entry_t {
                     return {v.first, v.second};
                  });

   std::sort(sranks.rbegin(), sranks.rend(), std::less<rank_entry_t>());

   return sranks;
}


void print_ranks(const sorted_ranks_t& ranks)
{
   auto it = ranks.begin();

   while (it != ranks.end()) 
   {
      const auto& entry = *(it++);
      std::cout << entry.candidate_id << ": " << entry.rank << std::endl;
   }
}


