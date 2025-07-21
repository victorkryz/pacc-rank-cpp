#include "protocol_processor.h"
#include "downloader.h"
#include <algorithm>
#include <exception>
#include <map>
#include <sstream>
#include <string_view>

using namespace std::literals;

ProtocolProcessor::ProtocolProcessor(std::shared_ptr<ContentProvider>& protocol_provider,
                                     std::ostream& output_dev) : protocol_provider_(protocol_provider),
                                                                 output_dev_(output_dev) {}

ProtocolProcessor::~ProtocolProcessor()
{
    if (protocol_.is_open())
        protocol_.close();
}

void ProtocolProcessor::process()
{
    obtain_protocol();

    auto ranks = parse_protocol();
    ranks_ = sort_ranks(ranks);
}

void ProtocolProcessor::print_report()
{
    auto it = ranks_.begin();

    std::uint16_t order(1);
    while (it != ranks_.end())
    {
        const auto [candidate_id, rank] = *(it++);
        output_dev_ << order++ << ") " << candidate_id << " - " << rank << std::endl;
    }
}

void ProtocolProcessor::obtain_protocol()
{
    if (!protocol_.is_open())
    {
        protocol_ = protocol_provider_->obtain_content();
        if (!protocol_.good())
            std::runtime_error("Bad protocol content");
    }
    else if (!protocol_.good())
    {
        protocol_.clear();
        protocol_.seekg(0, std::ios_base::beg);
    }
}

ProtocolProcessor::ranks_t ProtocolProcessor::parse_protocol()
{
    constexpr auto RANK_PREFIX = "V="sv;
    constexpr char RANK_END_DELIMITER = ' ';

    protocol_.clear();
    protocol_.seekg(0, std::ios_base::beg);

    ranks_t ranks;
    for (std::string line; std::getline(protocol_, line);)
    {
        size_t rank_pos = line.find(RANK_PREFIX);
        if (rank_pos != std::string::npos)
        {
            rank_pos += std::size(RANK_PREFIX);

            const size_t rank_end_pos = line.find_first_of(RANK_END_DELIMITER, rank_pos);
            const std::string rank_sequence = line.substr(rank_pos, rank_end_pos);

            extract_rank_values(rank_sequence, ranks);
        }
    }

    return ranks;
}

void ProtocolProcessor::extract_rank_values(const std::string& rank_sequence,
                                            ProtocolProcessor::ranks_t& ranks)
{
    constexpr char RANK_VALUES_DELIMITER = ',';

    std::stringstream s_stream(rank_sequence);
    std::string s_value;
    while (std::getline(s_stream, s_value, RANK_VALUES_DELIMITER))
    {
        const int rank_val = std::stoi(s_value);
        ranks_t::value_type::second_type& count = ranks[rank_val];
        if (!count.has_value())
            count = 1;
        else
            count.value()++;
    }
}

ProtocolProcessor::sorted_ranks_t ProtocolProcessor::sort_ranks(const ProtocolProcessor::ranks_t& ranks)
{
    sorted_ranks_t sranks;
    sranks.resize(ranks.size());

    std::transform(ranks.begin(), ranks.end(),
                   sranks.begin(), [](const ranks_t::value_type& v) -> rank_entry_t
                   { return {v.first, v.second.value()}; });

    std::sort(sranks.rbegin(), sranks.rend(), std::less<rank_entry_t>());

    return sranks;
}
