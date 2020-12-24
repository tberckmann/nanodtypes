
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
        return (key >> (4 - bit_off)) & 1

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
                self.l = self.l.insert_step(diff_bit, diff_key, new_key, new_item, self.bit)
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
        return str(self.item) + str(self.bit)

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

    def graph_subtrie(self):
        """Print the entire trie, recursively, as DOT commands, one edge per line"""
        if self.l:
            if self.l.bit <= self.bit:
                print(self._dispname(), "->", self.l._dispname(), "[style=dotted, tailport=sw, headport=s];")
            else:
                print(self._dispname(), "->", self.l._dispname(), "[tailport=sw];")
                self.l.graph_subtrie()
        if self.r:
            if self.r.bit <= self.bit:
                print(self._dispname(), "->", self.r._dispname(), "[style=dotted, tailport=se, headport=s];")
            else:
                print(self._dispname(), "->", self.r._dispname(), "[tailport=se];")
                self.r.graph_subtrie()

class PatriciaTrie:
    """Complete Patricia trie"""

    def __init__(self):
        self.head = None

    def print_text(self):
        """Print the trie in text form"""
        if self.head:
            self.head.print_subtrie()

    def print_graph(self):
        """Print a graphical representation of the trie in the DOT language"""
        print("digraph G {")
        if self.head:
            self.head.graph_subtrie()
        print("}")

    def lookup_key(self, key):
        """Look up a given key and return its value; if it doesn't exist, return None"""
        closest_link, parent_link = self.head.lookup_step(key, None)

        if closest_link.key == key:
            return closest_link.item
        else:
            return None

    @staticmethod
    def _find_bit_diff(key1, key2):
        """Find first bit difference between two keys"""
        for bit in range(5):
            if Link.bit_set(bit, key1) != Link.bit_set(bit, key2):
                return bit

    def insert_key(self, new_key, new_item):
        """Insert a new key-value pair into the trie

        If it already exists, do nothing
        """
        if self.head is None:
            bit_diff = self._find_bit_diff(new_key, 0)
            start_link = Link(new_key, new_item, bit_diff)
            start_link.r = start_link
            self.head = start_link
        else:
            closest_link, parent_link = self.head.lookup_step(new_key, None)
            if closest_link.key == new_key:
                return
            tgt_bit = self._find_bit_diff(new_key, closest_link.key)
            self.head = self.head.insert_step(tgt_bit, closest_link.key, new_key, new_item, -1)


    def remove_key(self, victim_key):
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


def key_from_let(let):
    return ord(let) & 0x1F

# Finished trie

built_trie = PatriciaTrie()

built_trie.insert_key(key_from_let("A"), "A")
built_trie.insert_key(key_from_let("S"), "S")
built_trie.insert_key(key_from_let("E"), "E")
built_trie.insert_key(key_from_let("R"), "R")
built_trie.insert_key(key_from_let("C"), "C")
built_trie.insert_key(key_from_let("H"), "H")
built_trie.insert_key(key_from_let("I"), "I")
built_trie.insert_key(key_from_let("N"), "N")

built_trie.remove_key(key_from_let("S"))
built_trie.remove_key(key_from_let("A"))

built_trie.print_graph()
