#include <spacepi/liblinux/Image.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/PartitionTable.hpp>
#include <spacepi/liblinux/steps/FormatPartitionsStep.hpp>

using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void FormatPartitionsStep::run(InstallationData &data) {
    Image &i = data.getData<Image>();
    PartitionTable &pt = data.getData<PartitionTable>();
    i.formatPartitions(pt);
}
