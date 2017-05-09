from os import path
import re

_THIS_FILE = path.realpath(__file__)
_THIS_DIR = path.dirname(_THIS_FILE)

OP_C = path.abspath(path.join(_THIS_DIR, "../../src/ops/op.c"))


def list_tele_ops():
    """Return the names of all the structs defined in tele_ops"""
    with open(OP_C, "r") as f:
        op_c = _remove_comments(f.read())
        ops = re.findall("&op_[a-zA-Z0-9_]+", op_c)
        ops = [s[1:] for s in ops]
        return ops


def list_tele_mods():
    """Return the names of all the structs defined in tele_mods"""
    with open(OP_C, "r") as f:
        op_c = _remove_comments(f.read())
        mods = re.findall("&mod_[a-zA-Z0-9_]+", op_c)
        mods = [s[1:] for s in mods]
        return mods


def _remove_comments(op_c):
    out = op_c.splitlines()
    out = filter(_is_not_comment, out)
    return "\n".join(out)


def _is_not_comment(line):
    s = line.lstrip()
    return not (s.startswith("//") or s.startswith("/*"))
