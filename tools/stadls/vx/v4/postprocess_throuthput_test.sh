#!/bin/bash

tmp=$(mktemp)
./bin/stadls_swtest_vx_v4 --loglevel=trace &> $tmp

echo "write_messages_per_s = {"
cat $tmp | grep UT | sed 's,.*stadls\.encode_throughput ,,g' | sort -k3 -n | grep "write:" | sed 's, write:,":,g' | sed 's,^,",g' | sed "s/ \[.*/,/g"
echo "}"

echo "write_num_messages_per_container = {"
cat $tmp | sed 's,.*stadls\.encode_throughput ,,g' | grep "write size:" | sort -k3 -n |  sed 's, write size:,":,g' | sed 's,^,",g' | sed "s/$/,/g"
echo "}"

echo "write_num_locations = {"
cat $tmp | sed 's,.*stadls\.encode_throughput ,,g' | grep "write coord size:" | sort -k3 -n | sed 's, write coord size:,":,g' | sed 's,^,",g' | sed "s/$/,/g"
echo "}"

echo "read_messages_per_s = {"
cat $tmp | grep UT  | sed 's,.*stadls\.encode_throughput ,,g'| sort -k3 -n | grep "read:" | sed 's, read:,":,g' | sed 's,^,",g' | sed "s/ \[.*/,/g"
echo "}"

echo "read_num_messages_per_container = {"
cat $tmp | sed 's,.*stadls\.encode_throughput ,,g' | grep "read size:" | sort -k3 -n |  sed 's, read size:,":,g' | sed 's,^,",g' | sed "s/$/,/g"
echo "}"

echo "read_num_locations = {"
cat $tmp | sed 's,.*stadls\.encode_throughput ,,g' | grep "read coord size:" | sort -k3 -n | sed 's, read coord size:,":,g' | sed 's,^,",g' | sed "s/$/,/g"
echo "}"

echo "read_get_messages_per_s = {"
cat $tmp | grep UT | sed 's,.*stadls\.encode_throughput ,,g' | sort -k3 -n | grep "readget:" | sed 's, readget:,":,g' | sed 's,^,",g' | sed "s/ \[.*/,/g"
echo "}"

echo "read_get_num_messages_per_container = {"
cat $tmp | sed 's,.*stadls\.encode_throughput ,,g' | grep "readget size:" | sort -k3 -n |  sed 's, readget size:,":,g' | sed 's,^,",g' | sed "s/$/,/g"
echo "}"

echo "read_get_num_locations = {"
cat $tmp | sed 's,.*stadls\.encode_throughput ,,g' | grep "readget coord size:" | sort -k3 -n | sed 's, readget coord size:,":,g' | sed 's,^,",g' | sed "s/$/,/g"
echo "}"

rm $tmp

cat << EOF
from matplotlib import pyplot as plt

write_throughput = []
write_num_messages_per_chip = []
for name, value in write_messages_per_s.items():
    write_throughput.append(value * 8)
    write_num_messages_per_chip.append(write_num_messages_per_container[name])

read_throughput = []
read_num_messages_per_chip = []
for name, value in read_messages_per_s.items():
    read_throughput.append(value * 8)
    read_num_messages_per_chip.append(read_num_messages_per_container[name])

read_get_throughput = []
read_get_num_messages_per_chip = []
for name, value in read_get_messages_per_s.items():
    read_get_throughput.append(value * 8)
    read_get_num_messages_per_chip.append(read_get_num_messages_per_container[name])

plt.plot(write_num_messages_per_chip, write_throughput, ls='', marker='o', label='write')
plt.plot(read_num_messages_per_chip, read_throughput, ls='', marker='x', label='read')
plt.plot(read_get_num_messages_per_chip, read_get_throughput, ls='', marker='x', label='read ticket.get()')
plt.axhline(125e6, label="1 GBit", ls='--')
plt.xlabel("#UT-Messages per container")
plt.ylabel("Encoding/decoding rate [B/s]")
plt.legend()
plt.xscale('log')
plt.yscale('log')
plt.savefig("throughput_write_read.pdf")

def get_duration_chip(messages_per_s, num_messages_per_container, num_locations):
    total = 0.
    for name, frequency in messages_per_s.items():
        if any(sub in name for sub in ["Label", "Weight", "CorrelationCalib"]):
            continue
        if "Row" in name and not "CADCSample" in name:
            continue
        if "CADCSamples" in name:
            continue
        if "::PPUMemoryBlock" in name:
            continue
        if "Byte" in name:
            continue
        if "Word" in name:
            continue
        if "Quad" in name:
            continue
        if "DAC" in name and not "Block" in name:
            continue
        if "Cell" in name:
            continue
        if "::NeuronBackendConfig" in name:
            continue
        if "::NeuronConfig" in name:
            continue
        total = total + num_messages_per_container[name] * num_locations[name] / frequency
    return total

print("chip write encode:", get_duration_chip(write_messages_per_s, write_num_messages_per_container, write_num_locations), "s")
print("chip read encode:", get_duration_chip(read_messages_per_s, read_num_messages_per_container, read_num_locations), "s")
print("chip read decode:", get_duration_chip(read_get_messages_per_s, read_get_num_messages_per_container, read_get_num_locations), "s")
EOF
