# 🧠 NoraSearch — PostgreSQL Extension

**NoraSearch** is a PostgreSQL C++ extension that performs a *solid search* operation between two text strings.  
It compares all character alignments between a main string and a substring, returning offsets where multiple matching characters occur.

This extension is a C++ port of a Go function originally designed for position-based substring correlation (`solidSearch`), now adapted to PostgreSQL with efficient native performance.

---

## 🚀 Features

- Fast C-level implementation of a substring correlation search  
- Returns all positive offsets where matching character alignments exceed a threshold  
- Compatible with PostgreSQL arrays (`int[][]` return type)  
- Easy to build and install using `pg_config` and `PGXS`  
- Safe, self-contained, and easy to extend  

---

## 🧩 Function Signature

```sql
noraSearch(str text, substr text, minmatch integer) → int[][]
