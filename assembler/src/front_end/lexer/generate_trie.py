#!/usr/bin/env python3
from collections import defaultdict
from typing import Dict, List

# --------------------------
# Konfiguration
# --------------------------

# Liste der Tokens: ("Mnemonic", "ENUM_NAME")
TOKENS = [
    ("%", "TOKEN_PERCENT"),
    ("\n", "TOKEN_NEWLINE"),
    (",", "TOKEN_COMMA"),
    ("(", "TOKEN_LPAREN"),
    (")", "TOKEN_RPAREN"),
    (":", "TOKEN_COLON"),
    ("+", "TOKEN_PLUS"),
    ("-", "TOKEN_MINUS"),
    ("+", "TOKEN_STAR"),
    ("+", "TOKEN_SLASH"),
    ("+", "TOKEN_SHL"),
    ("+", "TOKEN_SHR"),
    ("+", "TOKEN_AND"),
    ("+", "TOKEN_OR"),
    ("+", "TOKEN_XOR"),
    ("!", "TOKEN_NOT"),
]

CHAR_RANGE = 128  # A-Z

# --------------------------
# Trie Node Definition
# --------------------------
class Node:
    def __init__(self):
        self.children: Dict[str, int] = {}  # Buchstabe -> Index
        self.is_word: bool = False
        self.token: str = "TOKEN_UNKNOWN"

# --------------------------
# Trie aufbauen
# --------------------------
nodes: List[Node] = [Node()]  # node 0 = root

def insert(word: str, token: str):
    idx = 0  # Start bei root
    for c in word.upper():
        if c not in nodes[idx].children:
            nodes.append(Node())
            nodes[idx].children[c] = len(nodes) - 1
        idx = nodes[idx].children[c]
    nodes[idx].is_word = True
    nodes[idx].token = token

for word, token in TOKENS:
    insert(word, token)

# --------------------------
# Kind-Arrays für statisches C-Array erzeugen
# --------------------------
def generate_c_array():
    c_nodes = []
    for node in nodes:
        children_array = [-1]*CHAR_RANGE
        for c, child_idx in node.children.items():
            i = ord(c) - ord('A')
            children_array[i] = child_idx
        c_nodes.append((children_array, node.is_word, node.token))
    return c_nodes

c_nodes = generate_c_array()

# --------------------------
# C-Code ausgeben
# --------------------------
print("typedef struct {")
print("    const int children[26];")
print("    const bool is_word;")
print("    const TokenType token;")
print("} StaticTrieNode;")
print()
print(f"const StaticTrieNode trie[{len(c_nodes)}] = {{")

for children, is_word, token in c_nodes:
    children_str = ", ".join(str(x) for x in children)
    is_word_str = "true" if is_word else "false"
    print(f"    {{ {{ {children_str} }}, {is_word_str}, {token} }},")
print("};")