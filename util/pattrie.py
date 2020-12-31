
import string
import random
import os

# Prototyping code for Patricia trie

class Link:
    """Individual node in a Patricia trie"""

    def __init__(self, key=None, item=None, bit=None):
        self.l = self.r = None
        self.item = item
        self.bit = bit
        self.key = key

    @staticmethod
    def bit_set(bit_off, key):
        """Check if the bit offset in a given key is set to 1"""
        if bit_off == 5:
            return True
        return (key >> (4 - bit_off)) & 1

    @staticmethod
    def _find_bit_diff(key1, key2):
        """Find first bit difference between two keys"""
        for bit in range(5):
            if Link.bit_set(bit, key1) != Link.bit_set(bit, key2):
                return bit
        return 5

    # Basic operations on the trie

    def lookup_step(self, srch_key, parent_node):
        """Recursively search trie for a given key

        Returns: (link, parent_link) pair, where link is the last link examined
        and parent_link is its parent. If the link's key is not equal to the
        search key, then the key isn't in the trie.
        """

        prev_offset = -1 if parent_node is None else parent_node.bit

        if self.bit <= prev_offset:
            return (self, parent_node)

        if self.bit_set(self.bit, srch_key):
            if self.r:
                return self.r.lookup_step(srch_key, self)
            else:
                return (self, parent_node)
        else:
            if self.l:
                return self.l.lookup_step(srch_key, self)
            else:
                return (self, parent_node)

    def insert_step(self, diff_bit, diff_key, new_key, new_item, parent_bit):
        """Recursively insert a new key/item pair into the trie

        Assumes the item does not exist.

        diff_bit: the bit position in which the new key differs from the closest key
            already in the trie
        diff_key: the closest key already in the trie to the new key
        new_key: the key to be added
        new_item: the item to be added
        parent_bit: the deciding bit for the parent node of self
        """
        if self.bit > diff_bit or self.bit <= parent_bit:
            new_link = Link(key=new_key, item=new_item, bit=diff_bit)
            if self.bit_set(diff_bit, new_key):
                new_link.r = new_link
                new_link.l = self
            else:
                new_link.r = self
                new_link.l = new_link
            return new_link
        else:
            if self.bit_set(self.bit, new_key):
                self.r = self.r.insert_step(diff_bit, diff_key, new_key, new_item, self.bit)
            else:
                if self.l:
                    self.l = self.l.insert_step(diff_bit, diff_key, new_key, new_item, self.bit)
                else:
                    new_bit_diff = self._find_bit_diff(new_key, 0)
                    new_link = Link(key=new_key, item=new_item, bit=new_bit_diff)
                    new_link.r = new_link
                    new_link.l = None
                    self.l = new_link
            return self

    @staticmethod
    def link_swap(grandchild_link, child_link, parent_link):
        """Remove the child_link from consideration

        Move the child's key to the grandchild and update the parent to point
        to the grandchild. Note that the parent and the child could be the same link.
        """

        # First move the key and item
        grandchild_link.key = child_link.key
        grandchild_link.item = child_link.item
        child_link.key = 9 # Debug
        child_link.item = "X" # Debug

        # Move pointer
        if parent_link.l is child_link:
            parent_link.l = grandchild_link
        else:
            parent_link.r = grandchild_link

        # Debug
        if child_link.r is grandchild_link:
            child_link.r = None
        else:
            child_link.l = None

    def reduce_link(self, victim_link, target_key):
        """Recursively remove a null link from the trie

        The null link is assumed to have only one child
        """
        new_child = victim_link.l if victim_link.l else victim_link.r

        if self.l is victim_link:
            self.l = new_child
            return
        elif self.r is victim_link:
            self.r = new_child
            return

        if self.bit_set(self.bit, target_key):
            return self.r.reduce_link(victim_link, target_key)
        else:
            return self.l.reduce_link(victim_link, target_key)

    # Graphing / printing functions

    def _dispname(self):
        """Return the display label for this link"""
        return '"' + str(self.item) + str(self.bit) + '"'

    def _print_terminal(self, indent):
        """Print a leaf node on a single line, with indent"""
        leader = " " * indent
        print(leader + "Terminal: Item:", self.item, "bit", self.bit)

    def print_subtrie(self, indent=0):
        """Print the entire trie, recursively, in text, one node per line"""
        leader = " " * indent
        print(leader + "Item:", self.item, "bit:", self.bit)
        if self.l:
            if self.l.bit <= self.bit:
                self.l._print_terminal(indent + 1)
            else:
                self.l.print_subtrie(indent + 1)
        if self.r:
            if self.r.bit <= self.bit:
                self.r._print_terminal(indent + 1)
            else:
                self.r.print_subtrie(indent + 1)

    def graph_subtrie(self, tok_list=None):
        """Return a string representation of trie as DOT commands, one edge per line"""

        if tok_list is None:
            local_tok_list = []
        else:
            local_tok_list = tok_list

        if self.l:
            if self.l.bit <= self.bit:
                my_toks = [self._dispname(), "->", self.l._dispname(),
                    "[style=dotted, tailport=sw, headport=s];"]
                local_tok_list.append(my_toks)
            else:
                my_toks = [self._dispname(), "->", self.l._dispname(), "[tailport=sw];"]
                local_tok_list.append(my_toks)
                self.l.graph_subtrie(local_tok_list)
        if self.r:
            if self.r.bit <= self.bit:
                my_toks = [self._dispname(), "->", self.r._dispname(),
                    "[style=dotted, tailport=se, headport=s];"]
                local_tok_list.append(my_toks)
            else:
                my_toks = [self._dispname(), "->", self.r._dispname(), "[tailport=se];"]
                local_tok_list.append(my_toks)
                self.r.graph_subtrie(local_tok_list)

        if tok_list is None:
            final_string = ""
            for tok_line in local_tok_list:
                next_string_line = " ".join(tok_line) + "\n"
                final_string += next_string_line
            return final_string

class PatriciaTrie:
    """Complete Patricia trie"""

    def __init__(self):
        self.head = None

    def print_text(self):
        """Print the trie in text form"""
        if self.head:
            self.head.print_subtrie()

    def get_graph(self):
        """Print a graphical representation of the trie in the DOT language"""
        out = "digraph G {\n"
        if self.head:
            out += self.head.graph_subtrie()
        out += "}\n"
        return out

    def lookup_key(self, key):
        """Look up a given key and return its value; if it doesn't exist, return None"""
        if self.head is None:
            return None

        closest_link, parent_link = self.head.lookup_step(key, None)

        if closest_link.key == key:
            return closest_link.item
        else:
            return None

    def insert_key(self, new_key, new_item):
        """Insert a new key-value pair into the trie

        If it already exists, do nothing
        """
        if self.head is None:
            bit_diff = Link._find_bit_diff(new_key, 0)
            start_link = Link(new_key, new_item, bit_diff)
            start_link.r = start_link
            self.head = start_link
        else:
            closest_link, parent_link = self.head.lookup_step(new_key, None)
            if closest_link.key == new_key:
                return
            tgt_bit = Link._find_bit_diff(new_key, closest_link.key)
            self.head = self.head.insert_step(tgt_bit, closest_link.key, new_key, new_item, -1)

    def remove_key(self, victim_key):
        """Remove a key from the trie

        If it doesn't exist, do nothing.
        """
        closest_link, parent_link = self.head.lookup_step(victim_key, None)

        if closest_link.key != victim_key:
            return # Key didn't exist

        parent_link2, grandparent_link = self.head.lookup_step(parent_link.key, None)
        assert parent_link2 is parent_link

        if closest_link is parent_link:
            if parent_link.l is closest_link:
                parent_link.l = None
            else:
                parent_link.r = None

            if closest_link is self.head:
                self.head = closest_link.l if closest_link.l else closest_link.r
            else:
                self.head.reduce_link(closest_link, victim_key)
        else:
            Link.link_swap(closest_link, parent_link, grandparent_link)
            self.head.reduce_link(parent_link, victim_key)

class TestPatriciaTrie:

    @staticmethod
    def let_universe():
        return list(string.ascii_uppercase + " ")

    @staticmethod
    def _key_from_let(let):
        return ord(let) & 0x1F

    @staticmethod
    def binkey_from_let(let):
        """String representation in binary"""
        the_key = TestPatriciaTrie._key_from_let(let)
        return "{:0>5.5}".format(bin(the_key)[2:])

    def __init__(self):
        self.trie = PatriciaTrie()
        self.not_in = set(TestPatriciaTrie.let_universe()) # Letters not in the trie
        self.is_in = set() # Letters in the trie
        self.let_universe = set(TestPatriciaTrie.let_universe()) # All possible letters

    def _integrity_check(self):
        for let in self.not_in:
            key = self._key_from_let(let)
            assert self.trie.lookup_key(key) is None
        for let in self.is_in:
            key = self._key_from_let(let)
            assert self.trie.lookup_key(key) == let

    def insert_key(self, new_let):
        """Insert a letter into the trie with integrity check"""
        assert new_let in self.let_universe
        new_key = self._key_from_let(new_let)
        self.trie.insert_key(new_key, new_let)
        self.not_in.remove(new_let)
        self.is_in.add(new_let)
        #self._dump_graph("dumped/ins_" + new_let)
        self._integrity_check()

    def remove_key(self, victim_let):
        """Remove a letter into the trie with integrity check"""
        victim_key = self._key_from_let(victim_let)
        self.trie.remove_key(victim_key)
        self.not_in.add(victim_let)
        self.is_in.remove(victim_let)
        self._integrity_check()

    def _dump_graph(self, filename):
        with open(filename + ".gv", "w") as f_h:
            f_h.write(self.trie.get_graph())

        os.system("dot -Tps '%s.gv' -o '%s.ps'" % (filename,filename))

def book_sequence():
    """Build the tree according to the book"""
    test_trie = TestPatriciaTrie()
    let_sequence = ["A","S","E","R","C","H","I","N"]
    for let in let_sequence:
        test_trie.insert_key(let)

def rand_sequence():

    test_trie = TestPatriciaTrie()

    add_sequence = TestPatriciaTrie.let_universe()
    rem_sequence = TestPatriciaTrie.let_universe()

    random.shuffle(add_sequence)
    random.shuffle(rem_sequence)

    for let in add_sequence:
        print("Inserting", let, test_trie.binkey_from_let(let))
        test_trie.insert_key(let)

    for let in rem_sequence:
        print("Removing", let)
        test_trie.remove_key(let)

rand_sequence()
