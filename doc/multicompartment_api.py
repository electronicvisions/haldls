from copy import deepcopy
from pprint import pprint


class AnalogParameters(object):
    def __init__(self):
        self.i_bias_intercomp = 0
        self.v_reset = 200


class DigitalParameters(object):
    def __init__(self):
        self.connect_right = False
        self.connect_bottom = False
        self.connect_resistor = False
        self.connect_to_wire = False
        self.connect_wire = False


class Compartment(object):
    def __init__(self):
        self.analog_parameters = AnalogParameters()
        self.digital_parameters = DigitalParameters()

    def detach_from_wire(self):
        raise NotImplementedError

    def attach_switch_to_wire(self):
        """
        Connect this compartment to its adjacent wire using the
        lowest possible resistance.
        """
        self.digital_parameters.connect_to_wire = True
        self.digital_parameters.connect_resistor = False
        self.analog_parameters.i_bias_intercomp = 0

    def attach_resistor_to_wire(self, i_bias_intercomp):
        """
        Connect this compartment to its adjacent wire using the
        configurable inter-compartment resistor.
        """
        self.digital_parameters.connect_to_wire = False
        self.digital_parameters.connect_resistor = True
        self.analog_parameters.i_bias_intercomp = i_bias_intercomp

    def ascii_parts(self):
        hsw_repr = {False: "   ", True: "---"}
        vsw_repr = {False: "   ", True: "[|]"}

        if self.digital_parameters.connect_to_wire:
            vsw_symbol = "[|]"
        elif self.digital_parameters.connect_resistor:
            vsw_symbol = "[R]"
        else:
            vsw_symbol = "   "

        ascii_list = [
            "-+-{0}".format(hsw_repr[self.digital_parameters.connect_wire]),
            " |    ",
            "{0}   ".format(vsw_symbol),
            " |    ",
            "-N-{0}".format(hsw_repr[self.digital_parameters.connect_right]),
            " |    ",
            "{0}   ".format(vsw_repr[self.digital_parameters.connect_bottom]),
            " |    ",
        ]

        return ascii_list


class _CompartmentGetter(object):
    def __init__(self, compartments):
        self.compartments = compartments

    def __getitem__(self, item):
        assert len(item) == 2
        return self.compartments[item[0]][item[1]]


def _is_contiguous(coordinate_set):
    if not coordinate_set:
        return True

    test_set = set(coordinate_set)
    stack = {test_set.pop()}

    while stack:
        current = stack.pop()

        candidates = (
            (current[0] + 1, current[1]),
            (current[0] - 1, current[1]),
            (current[0], current[1] + 1),
            (current[0], current[1] - 1)
        )

        for cand in candidates:
            try:
                test_set.remove(cand)
            except KeyError:
                pass
            else:
                stack.add(cand)

    return not test_set


# NOTE: this implementation does not handle the mirroring
#       of the right half of the chip
class CompartmentBlock(object):
    WIDTH = 128
    HEIGHT = 2

    def __init__(self):
        self.compartments = [[Compartment() for _ in xrange(self.HEIGHT)]
                             for _ in xrange(self.WIDTH)]

    @property
    def compartment(self):
        return _CompartmentGetter(self.compartments)

    def connect_wires(self, start, end, level):
        """
        Connect all wire segments from start to end, including end.

        start, end: x-coordinate of the wire segments
        level: y-coordinate of the wire segment
        """

        for segment_idx in xrange(start, end):
            compartment = self.compartments[segment_idx][level]
            compartment.digital_parameters.connect_wire = True

    def connect_compartments(self, compartment_set):
        """
        Connect all compartments using the direct interconnection switches.

        Raises a RuntimeError if the segments can not be connected to
        one contiguous chunk.
        """
        if not compartment_set:
            return

        if not _is_contiguous(compartment_set):
            raise RuntimeError("compartment set is not contiguous")

        for coord in compartment_set:
            if (coord[0] + 1, coord[1]) in compartment_set:
                self.compartment[coord].digital_parameters.connect_right = True
            if coord[1] == 0 and (coord[0], coord[1] + 1) in compartment_set:
                self.compartment[coord].digital_parameters.connect_bottom = True
            if coord[1] == 1 and (coord[0], coord[1] - 1) in compartment_set:
                self.compartment[coord].digital_parameters.connect_bottom = True

    def ascii_repr(self, truncate=None):
        if truncate is None:
            comp_range = xrange(0, self.WIDTH)
            addr = slice(None, None, None)
        else:
            assert len(truncate) == 2
            assert truncate[0] <= truncate[1]
            comp_range = xrange(truncate[0], truncate[1])
            addr = slice(truncate[0], truncate[1], None)

        partial_upper = [comp[0].ascii_parts()
                         for comp in self.compartments[addr]]
        partial_lower = [comp[1].ascii_parts()[-2::-1]
                         for comp in self.compartments[addr]]

        numbers = "".join((" {:<5}".format(idx) for idx in comp_range))
        upper = "\n".join(("".join(ii) for ii in zip(*partial_upper)))
        lower = "\n".join(("".join(ii) for ii in zip(*partial_lower)))

        return numbers + "\n\n" + upper + "\n" + lower + "\n\n" + numbers


"""

  HICANN X: A tree structure with proximal and distal dendrites,
  inspired by [Fig. 7, Schemmel et al. 2017]

        [ distal  ] ["Ca"] [ more distal ]

    0     1     2     3     4     5     6     7

0 --+-   -+-----+-----+-----+-----+-----+-   -+--
    |     |     |     |     |     |     |     |
         [R]   [R]   [R]   [R]   [R]   [R]
    |     |     |     |     |     |     |     |
0 --N-   -N-   -N-   -N-   -N-   -N-   -N-   -N--
    |     |     |     |     |     |     |     |
                     [|]
                      |   <----  (yes, there are two switches here)
                     [|]
    |     |     |     |     |     |     |     |
1 --N-----N-----N-   -N-   -N-   -N-   -N-   -N--
    |     |     |     |     |     |     |     |
               [|]   [R]   [R]   [R]   [R]
    |     |     |     |     |     |     |     |
1 --+-   -+-   -+-----+-----+-----+-----+-   -+--

    0     1     2     3     4     5     6     7

   [ --- "Na" --- ] ["Ca"] [ proximal dend ]

"""

block = CompartmentBlock()

# "Ca"-compartment
block.compartment[3, 0].attach_resistor_to_wire(i_bias_intercomp=400)
# update resistor value:
block.compartment[3, 0].analog_parameters.i_bias_intercomp = 390
block.compartment[3, 0].analog_parameters.v_reset = 500
block.compartment[3, 1].attach_resistor_to_wire(i_bias_intercomp=400)

# abstract away that two switches have to be connected
block.connect_compartments([(3, 0), (3, 1)])

# Note that "bottom" may be
# confusing here: consider naming it "opposite" instead
assert block.compartment[3, 0].digital_parameters.connect_bottom
assert block.compartment[3, 1].digital_parameters.connect_bottom

# "Na"-compartment
block.connect_compartments([(0, 1), (1, 1), (2, 1)])

# e that "right" is actually
#  left on the right half of the chip -> consider renaming
assert (block.compartment[
    0, 1].digital_parameters.connect_right)

assert block.compartment[1, 1].digital_parameters.connect_right
assert not block.compartment[2, 1].digital_parameters.connect_right

block.compartment[2, 1].attach_switch_to_wire()
block.compartment[2, 1].analog_parameters.v_reset = 100

assert block.compartment[2, 1].digital_parameters.connect_to_wire

# distal dendrites
block.connect_wires(1, 6, 0)

for comp_idx in [1, 2, 4, 5, 6]:
    block.compartment[comp_idx, 0].attach_resistor_to_wire(i_bias_intercomp=200)
    block.compartment[comp_idx, 0].analog_parameters.v_reset = 550

# proximal dendrites
block.connect_wires(2, 6, 1)

for comp_idx in [4, 5, 6]:
    block.compartment[comp_idx, 1].attach_resistor_to_wire(i_bias_intercomp=300)
    block.compartment[comp_idx, 1].analog_parameters.v_reset = 550

print block.ascii_repr((0, 10))


"""
Second example (compatible with HICANN DLS)

Three compartments are connected in a chain using resistors:


 0     1     2     3     4     5

-+-----+-   -+-----+-   -+-   -+-
 |     |     |     |     |     |
[R]   [|]   [R]   [|]
 |     |     |     |     |     |
-N-   -N-----N-   -N-   -N-   -N-
 |     |     |     |     |     |


"""

block = CompartmentBlock()


block.connect_wires(0, 1, 0)
block.connect_wires(2, 3, 0)
block.connect_compartments([(1, 0), (2, 0)])

block.compartment[0, 0].attach_resistor_to_wire(291)
block.compartment[1, 0].attach_switch_to_wire()
block.compartment[2, 0].attach_resistor_to_wire(231)
block.compartment[3, 0].attach_switch_to_wire()

print
print block.ascii_repr((0, 6))


# mapping example #########################################################

from collections import namedtuple
from functools import total_ordering
from abc import ABCMeta

# --- coordinates: required for implementation ------------------------------


class Coordinate(object):
    __metaclass__ = ABCMeta


@total_ordering
class WireSegmentC(Coordinate):
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __lt__(self, other):
        if not isinstance(other, WireSegmentC):
            return False
        return (self.y, self.x) < (other.y, other.x)

    def __hash__(self):
        return (self.x, self.y).__hash__()

    def __eq__(self, other):
        if isinstance(other, WireSegmentC):
            return self.x == other.x and self.y == other.y
        return NotImplemented

    def __repr__(self):
        return "W(" + str(self.x) + ", " + str(self.y) + ")"


@total_ordering
class CompartmentC(Coordinate):
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __lt__(self, other):
        if not isinstance(other, CompartmentC):
            return False
        return (self.y, self.x) < (other.y, other.x)

    def __hash__(self):
        return (self.x, self.y).__hash__()

    def __eq__(self, other):
        if not isinstance(other, CompartmentC):
            return False
        return self.x == other.x and self.y == other.y

    def __repr__(self):
        return "C(" + str(self.x) + ", " + str(self.y) + ")"


# --- logical description: nodes, resistors and multicomp. neurons ----------

@total_ordering
class Node(object):
    def __init__(self, capacitance=1.0, name=None):
        self.capacitance = float(capacitance)
        self.name = name

    def __lt__(self, other):
        if not isinstance(other, Node):
            return False
        return self.name < other.name

    def __repr__(self):
        return "N(" + repr(self.name) + ")"

    def __hash__(self):
        return self.name.__hash__()

    # NOTE: this is purely implemented as a proof-of-concept and should be changed
    def __eq__(self, other):
        if isinstance(other, Node):
            return self.name == other.name
        return NotImplemented


class Resistor(object):
    def __init__(self, a, b, resistance=1.0):
        self.a = a
        self.b = b
        self.resistance = resistance

    def __repr__(self):
        return "R(" + str(self.a) + ", " + str(self.b) + ")"

    def __hash__(self):
        return (self.a, self.b).__hash__()

    # NOTE: this is purely implemented as a proof-of-concept and should be changed
    def __eq__(self, other):
        return isinstance(other, Resistor) and self.a == other.a and self.b == other.b


class MCNeuron(object):
    def __init__(self):
        self.nodes = []
        self.resistors = []

    def __str__(self):
        return ("MCNeuron(\n"
                + "    " + str(self.nodes) + ",\n"
                + "    " + str(self.resistors) + ")")

    def add_node(self, *args):
        new_node = Node(*args)
        self.nodes.append(new_node)
        return new_node

    def add_resistor(self, node_a, node_b):
        new_res = Resistor(node_a, node_b)
        self.resistors.append(new_res)
        return new_res

    def get_connected_nodes(self, node):
        """
        returns all nodes that are connected to `node`
        """
        for resistor in self.resistors:
            if resistor.a == node:
                yield resistor.b, resistor
            elif resistor.b == node:
                yield resistor.a, resistor


# --- mapping algorithm and required structures -----------------------------

class MappingState(object):
    """
    Holds the intermediate state of a multi-compartment neuron mapping.

    :cvar CONNECT_OPPOSITE_SIDE: whether to use both neuron block sides
        when searching for new connection possibilities.
    """
    CONNECT_OPPOSITE_SIDE = True

    def __init__(self):
        self.mapped_nodes = dict()
        self.mapped_resistors = dict()

    def mapping_complete(self, neuron):
        """
        :return: True if all nodes and edges of self.neuron
            have been mapped
        """
        return set(self.mapped_nodes.iterkeys()) == set(neuron.nodes)

    def verify_mapping(self, neuron):
        """
        Verify that all components have been mapped correctly.

        :return: True if the mapping is correct: All resistors are
            mapped and connect their corresponding nodes; no nodes
            are connected that do not have a corresponding resistor
            in `neuron`.
            No component is mapped twice.
        """

        # This function is not fully implemented
        raise NotImplementedError

        all_mapped = set()

        for mapped_components in self.mapped_nodes.itervalues():
            if mapped_components & all_mapped:
                return False
            all_mapped |= mapped_components

        return True


    def component_is_free(self, component):
        """
        :type component: Coordinate
        :param component: The component to be checked.
        :return: True if the component is free to use, False otherwise.
            This includes the usage status by the current state of
            the mapping as well as block boundaries and possible
            blacklisted components
        """
        if component.x < 0:
            return False
        if component.x >= CompartmentBlock.WIDTH:
            return False

        # NOTE: One can check for blacklisting here, e.g.:
        #
        # if blacklisting.contains(component):
        #     return False
        #

        # Note: Resistors are always mapped to compartments which
        #     are also mapped to nodes. Thus it is not required to
        #     check them here.

        for mapped_components in self.mapped_nodes.itervalues():
            if component in mapped_components:
                return False

        return True

    def components_are_free(self, components):
        for component in components:
            if not self.component_is_free(component):
                return False

        return True

    def iterate_next_mappable(self, neuron):
        """
        select the next mappable node

        :return: A tuple consisting of an already mapped node,
            the new node and the resistor connecting the two.
        :rtype: (Node, Node, Resistor)
        """

        for mapped_node in self.mapped_nodes.iterkeys():
            for connected_node, resistor in neuron.get_connected_nodes(mapped_node):
                if connected_node not in self.mapped_nodes:
                    yield mapped_node, connected_node, resistor

    def iterate_free_attach_sites(self, mapped_node, neuron):
        """
        Generate possible sites to attach a new node using
        a resistor.

        :param neuron: The neuron that is being mapped
        :type neuron: MCNeuron
        :param mapped_node: A set of coordinates that are used by
            the already mapped node.
        :type mapped_node: Node
        :return: Iterator over the possible attach sites.
            Each element is a tuple of three components:
            - a set of coordinates by which to exdend the mapped node
            - a set of coordinates which comprise the newly created node
            - the coordinate of the compartment in which the newly mapped
              resistor resides.
        :rtype: collections.Iterable[set[Coordinate], set[Coordinate], CompartmentC]
        """
        assert mapped_node in self.mapped_nodes

        mapped_node_components = self.mapped_nodes[mapped_node]

        for component in mapped_node_components:
            if isinstance(component, WireSegmentC):
                candidate = CompartmentC(component.x, component.y)
                if self.component_is_free(candidate):
                    yield set(), {candidate}, candidate

                for dx in [1, -1]:
                    compartment = CompartmentC(component.x + dx, component.y)
                    add_wire = WireSegmentC(component.x + dx, component.y)
                    candidate = {compartment,
                                 add_wire}
                    if self.components_are_free(candidate):
                        yield {add_wire}, {compartment}, compartment

            if isinstance(component, CompartmentC):
                candidate_segment = WireSegmentC(component.x, component.y)

                if self.component_is_free(candidate_segment):
                    additional_wire = {candidate_segment}

                    right_wire = WireSegmentC(component.x + 1, component.y)
                    right_compartment = CompartmentC(component.x + 1, component.y)

                    left_wire = WireSegmentC(component.x - 1, component.y)
                    left_compartment = CompartmentC(component.x - 1, component.y)

                    required_right = {right_wire, right_compartment}
                    required_left = {left_wire, left_compartment}

                    if self.components_are_free(required_right):
                        yield additional_wire | {right_wire}, {right_compartment}, right_compartment
                    if self.components_are_free(required_left):
                        yield additional_wire | {left_wire}, {left_compartment}, left_compartment

            if self.CONNECT_OPPOSITE_SIDE:
                if isinstance(component, CompartmentC):
                    for dx in [1, -1]:
                        additional_wire = {
                            CompartmentC(component.x, 1 - component.y),
                            WireSegmentC(component.x, 1 - component.y),
                            WireSegmentC(component.x + dx, 1 - component.y)}

                        new_compartment = CompartmentC(component.x + dx, 1 - component.y)

                        required_components = additional_wire | {new_compartment}
                        if self.components_are_free(required_components):
                            yield additional_wire, {new_compartment}, new_compartment


def map_neuron_rec(mapping_state, neuron):
    """

    Recursively map `neuron` to a valid hardware configuration.

    :param mapping_state:
    :type mapping_state: MappingState
    :param neuron:
    :type neuron: MCNeuron
    :return:
    :rtype: generator
    """

    assert mapping_state.mapped_nodes, "At least one compartment must be mapped"

    if mapping_state.mapping_complete(neuron):
        yield mapping_state
        return

    for mapped_node, new_node, new_res in mapping_state.iterate_next_mappable(neuron):

        for extend_old_node, new_node_components, new_resistor in mapping_state.iterate_free_attach_sites(mapped_node, neuron):
            new_state = deepcopy(mapping_state)

            new_state.mapped_nodes[mapped_node] |= extend_old_node
            new_state.mapped_nodes[new_node] = new_node_components
            new_state.mapped_resistors[new_res] = new_resistor

            for result in map_neuron_rec(new_state, neuron):
                yield result


def create_config_from_mapping(mapping):
    """
    :type mapping: MappingState
    :rtype: CompartmentBlock
    """
    block = CompartmentBlock()

    for key, value in mapping.mapped_nodes.iteritems():
        allcomps = set(((comp.x, comp.y)
                    for comp in value
                    if isinstance(comp, CompartmentC)))
        block.connect_compartments(allcomps)

        all_wires = sorted([
            wire
            for wire in value
            if isinstance(wire, WireSegmentC)])

        for wire in all_wires:
            if (wire.x, wire.y) in allcomps:
                block.compartment[wire.x, wire.y].attach_switch_to_wire()

        for pre, post in zip(all_wires[:-1], all_wires[1:]):
            if pre.y == post.y:
                if abs(pre.x - post.x) == 1:
                    block.connect_wires(pre.x, post.x, post.y)

    for key, s in mapping.mapped_resistors.iteritems():
        block.compartment[s.x, s.y].attach_resistor_to_wire(key.resistance)

    return block


def map_neuron(neuron, start_compartment):
    """
    :param neuron:
    :type neuron: MCNeuron

    :param start_compartment: Where to map the first compartment to.
    :type start_compartment: Coordinate

    :return: collections.Iterable[MappingState]
    """
    mapping_state = MappingState()

    initial_node = neuron.nodes[0]

    # We currently assume that the starting compartment is not blacklisted
    assert mapping_state.component_is_free(start_compartment)

    mapping_state.mapped_nodes[initial_node] = {start_compartment}

    for ii in map_neuron_rec(mapping_state, neuron):
        yield ii


# --- usage example ---------------------------------------------------------

import itertools


def example():
    neuron = MCNeuron()

    soma = neuron.add_node(1, "soma")
    ca = neuron.add_node(1, "ca")

    # NOTE: Mapping this node to a wire segment is not supported in this
    # example
    dummy = neuron.add_node(1, "dummy")

    distal1 = neuron.add_node(1, "d1")
    distal2 = neuron.add_node(1, "d2")

    proximal1 = neuron.add_node(1, "p1")
    proximal2 = neuron.add_node(1, "p2")

    neuron.add_resistor(soma, ca)
    neuron.add_resistor(ca, dummy)
    neuron.add_resistor(dummy, distal1)
    neuron.add_resistor(dummy, distal2)
    neuron.add_resistor(soma, proximal1)
    neuron.add_resistor(soma, proximal2)

    for idx, mapping in enumerate(map_neuron(neuron, CompartmentC(2, 0))):
        print
        print " ===", idx, "=" * 50
        print
        print "Node mapping:"
        pprint(mapping.mapped_nodes)
        print

        print "Resistor mapping:"
        pprint(mapping.mapped_resistors)
        print
        cfg = create_config_from_mapping(mapping)

        mincomp = min((ii.x
                       for ii in
                       itertools.chain(*mapping.mapped_nodes.itervalues())))
        maxcomp = max((ii.x
                       for ii in
                       itertools.chain(*mapping.mapped_nodes.itervalues())))

        print "Configuration schematic:"
        print
        print cfg.ascii_repr(truncate=(mincomp, maxcomp + 1))

        if idx > 100:
            break


if __name__ == '__main__':
    example()
