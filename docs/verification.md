# Verification

The imported sources are the output of strict verification run v17. Each function was compiled alone at its real address. Every instruction word was compared unmasked except relocations into the function's own local constant sections; float literals and switch-table contents were compared independently. Named references were resolved and recorded in `NAMES.tsv`. All rows published here have status `MATCH` and verdict `exact`.
