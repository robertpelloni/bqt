# Repository Hygiene Audit

## Summary
A previous broad namespace rename (`qt` -> `bobui`) appears to have generated a very large volume of **untracked artifacts** across the legacy C++ tree.

These artifacts include:
- renamed documentation files
- renamed CMake support files
- renamed test assets
- renamed generated/support headers
- renamed tool wrappers

## Current Risk
Blindly staging these files would be dangerous because it is not yet verified whether they are:
1. intended source of truth,
2. generated artifacts,
3. duplicate renamed copies,
4. or partial/inconsistent renames.

## Recommendation
Do **not** bulk-add them.

Instead:
1. identify whether they are generated vs canonical,
2. compare against tracked originals,
3. decide which namespace changes belong in source and which belong only in branding/docs,
4. add ignore rules or cleanup scripts where appropriate.

## Immediate Safe Path
- Continue working in the verified Go tree and curated docs.
- Treat the untracked C++ rename explosion as a separate cleanup project.
- Keep shell concerns in `bobfilez`.
