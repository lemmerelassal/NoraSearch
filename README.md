# 🧩 NoraSearch — PostgreSQL Extension

**NoraSearch** is a high-performance PostgreSQL extension written in C that efficiently finds substring match offsets and counts — based on a distance-based substring matching algorithm inspired by Go.

It returns every position (`offset`) where parts of one string match another, along with the number of matching characters.

---

## ⚙️ Build Instructions (Windows / MSYS2 MINGW64)

1. Open **MSYS2 MINGW64**.

2. Navigate to the folder containing this source code:

   ```bash
   cd path/to/norasearch
   ```

3. Build the extension:

   ```bash
   make
   ```

   ✅ This produces the compiled library:
   ```
   norasearch.dll
   ```

---

## 📦 Installation

Copy the following files to your PostgreSQL installation directories (adjust the version number if needed):

| File | Destination |
|------|--------------|
| `norasearch.dll` | `C:\Program Files\PostgreSQL\18\lib` |
| `norasearch.control` | `C:\Program Files\PostgreSQL\18\share\extension` |
| `norasearch--1.0.sql` | `C:\Program Files\PostgreSQL\18\share\extension` |

---

## 🧠 Usage

Once the files are in place, open **pgAdmin** or use **psql**, then run:

```sql
CREATE EXTENSION norasearch;
SELECT norasearch('abracadabra', 'abra', 2);
```

✅ **Expected Output:**

```text
{{0,4},{7,4}}
```

This means:

- At **offset 0**, 4 matching characters were found.  
- At **offset 7**, another 4 matching characters were found.

---

## 💡 How It Works

`norasearch` compares two strings byte-by-byte and counts how many characters align between them at each possible offset.

### Function Signature
```sql
norasearch(hay text, needle text, minmatch int) → int[][]
```

### Returns
A two-dimensional integer array of `{offset, count}` pairs:
```text
{{offset1, count1}, {offset2, count2}, ...}
```

where:
- **offset** = position in the first string where a match begins  
- **count** = number of matching characters  
- Only entries with `count > minmatch` are included

---

## 🧪 Examples

```sql
-- Basic example
SELECT norasearch('abracadabra', 'abra', 1);
-- → {{0,4},{7,4}}

-- Higher threshold
SELECT norasearch('banana', 'ana', 2);
-- → {{1,3}}

-- No matches above threshold
SELECT norasearch('postgres', 'sql', 0);
-- → NULL
```

---

## 🧰 Troubleshooting

- If you get an error like:
  ```
  ERROR:  could not load library "norasearch"
  ```
  Make sure:
  - `norasearch.dll` is copied into PostgreSQL’s `lib` directory.
  - You restarted PostgreSQL after installing.
  - File permissions allow access.

- To rebuild after making code changes:
  ```bash
  make clean && make
  ```

---

## 🧾 License

MIT License © 2025 — NoraSearch contributors

---

### ❤️ Inspired by Go

This extension ports a Go-based substring offset algorithm to C for PostgreSQL — giving SQL users the same efficient pattern-matching logic, directly inside the database engine.
