#pragma once
#include "hxcomm/common/hwdb_entry.h"
#include "hxcomm/vx/connection_from_env.h"

bool is_jboa_multichip_setup(auto& connection)
{
	// Check if all setups are JBOA-Setups.
	auto const hwdb_entries =
	    std::visit([](auto const& conn) { return conn.get_hwdb_entry(); }, connection);
	if (!std::all_of(hwdb_entries.begin(), hwdb_entries.end(), [](auto const& entry) {
		    return std::holds_alternative<hwdb4cpp::JboaSetupEntry>(entry);
	    })) {
		return false;
	}
	return true;
}

bool is_jboa_multichip_of_min_size(auto const& connection, size_t min_number_connections = 1)
{
	if (!is_jboa_multichip_setup(connection)) {
		return false;
	}
	size_t const connection_size =
	    std::visit([](auto const& conn) { return conn.size(); }, connection);
	return connection_size >= min_number_connections;
}