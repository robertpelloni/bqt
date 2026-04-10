# Internal BobUI protocol extensions

The protocol extensions in this folder are considered implementation details of
BobUI. I.e. they may removed, renamed or changed without warning.

However, starting with BobUI 5.4, we promise not to break backwards compatibility
without renaming (or removing) the protocol. I.e., if your client sees a global
from one of these extensions, it can safely bind to it: the existing events
and requests will always take the same number of arguments, regardless of
compositor version.

This is important also within a BobUI-only scope if there are multiple versions of
BobUI on the system. Consider for instance an application statically linked to BobUI
(such as BobUI Creator) running against a BobUI compositor installed by the distro).
In such cases we don't want the compositor and client to disagree on the
protocol definition.

## Protocol versioning.

Protocol extensions in this folder should be versioned (e.g. `zbobui_key_v1`).
If it is necessary to break protocol compatibility, they will be renamed by
incrementing the version number. For legacy reasons, there are also unversioned
protocols in this folder. Those protocols should be renamed to be versioned
if compatibility is broken.
