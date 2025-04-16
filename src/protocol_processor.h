#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <fstream>
#include "content_provider.h"

class ProtocolProcessor
{
    private:
        using ranks_t = std::unordered_map<std::uint32_t, std::optional<std::uint32_t>>;

        struct rank_entry_t
        {
            std::uint32_t candidate_id;
            std::uint32_t rank;

            bool operator<(const rank_entry_t &other) const
            {
                return rank < other.rank;
            }
        };

        using sorted_ranks_t = std::vector<rank_entry_t>;

    public:
        ProtocolProcessor(std::shared_ptr<ContentProvider>& protocol_provider, std::ostream &output_dev);
        ~ProtocolProcessor();

        void process();
        void print_report();

    protected:
         void obtain_protocol();

    private:
        ranks_t parse_protocol();
        void extract_rank_values(const std::string &rank_sequence, ranks_t &ranks);
        sorted_ranks_t sort_ranks(const ranks_t& ranks);

    private:
        std::ifstream protocol_;
        std::ostream &output_dev_;
        sorted_ranks_t ranks_;
        std::shared_ptr<ContentProvider> protocol_provider_;
};
