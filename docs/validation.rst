=================
NIX file validity
=================

Hard vs Soft rules
==================

The NIXIO libraries enforce the general NIX structure and requirements.  For instance, all data objects (Block, Group, DataArray, etc) must always have a name and a type and all the libraries and tools that can create NIX files enforce these rules.  Beyond the rules that the library enforces, there are also certain that are the responsibility of the user to follow.  These rules are not required in a working NIX file, but violating them is often indication that a file is *semantically invalid* or simply incomplete.  We can separate all the validity rules into two categories, *hard* and *soft* rules, depending on their severity.

*Hard rules* include the ones enforced by the library mentioned above, but also some rules that the users are expected to follow.  An example of these user-dependent *hard rules* is that the number of Dimension objects attached to a DataArray must match the number of dimensions in the data itself.  Violating this rule makes the DataArray *semantically invalid*, but it does not prevent the use of the file in any way.

*Soft rules* are less important than *hard rules*, as the name suggests.  Violating them does not make a file invalid, neither functionally nor semantically, but may limit certain functionality in the library or may be an indication of incomplete data descriptions.  An example of a *soft rule* is the absence of `units` in a DataArray, or the use of a non-SI unit.  This rule violation prevents the acquisition of data from a DataArray through a Tag or MultiTag object when the units of the objects are scaled versions of the each other.

This document lists both the *hard rules* and *soft rules* that are required for creating semantically valid and complete NIX files, separated by object type.

The NIX validator
=================

The NIXIO libraries implement a validator that can check a file for both soft and hard rule violations and inconsistencies.  The output of these validators label hard rule violations as *errors* and soft rule violations as *warnings*.

Hard rules
==========

File objects
------------
- Creation date must be set: HDF5 attribute `created_at`.

Entities
--------
- ID must be set: HDF5 attribute `entity_id`.
- Creation date must be set: HDF5 attribute `created_at`.
- Name must be set: HDF5 attribute `name`.
- Type must be set: HDF5 attribute `type`.

Block and Group objects
-----------------------
- Rules for `Entities`_ apply.

DataArray objects
-----------------
- Rules for `Entities`_ apply.
- Data type must be set: HDF5 attribute `dtype`.
- Number of attached Dimension objects must match the number of dimensions in the data.
- Number of Ticks in any attached RangeDimension objects must match the length of the corresponding dimension in the data.
- Number of Labels in any attached SetDimension objects, if set, must match the length of the corresponding dimension in the data.
  - Note that Labels may be empty.

Dimension objects
~~~~~~~~~~~~~~~~~
- Dimension index must be positive: HDF5 attribute `index`.
- Dimension type (attribute) must match Dimension object type: HDF5 attribute `dimension_type`.
- RangeDimension rules:
  - Ticks must be set: HDF5 dataset `ticks`.
  - Ticks must be sorted in increasing order.
  - Unit must be set to an atomic SI unit: HDF5 attribute `unit`.
- SampledDimension rules:
  - Sampling interval must be set and have a positive value: HDF5 attribute `sampling_interval`.
  - Unit must be set to an atomic SI unit: HDF5 attribute `unit`.

Tag objects
-----------
- Rules for `Entities`_ apply.
- Position must be set: HDF5 dataset `position`.
- If the Tag contains references, the length of `position` must match the number of dimensions in all the referenced DataArray objects.
- If the Tag contains an extent, the length of `extent` must match the number of dimensions in all the referenced DataArray objects (and the length of `position`).
- Units must be set to an atomic SI unit: HDF5 dataset `unit`.
- Number of units must match the number of dimensions in all the referenced DataArray objects.
- Units must be convertible to the corresponding units in all the referenced DataArray objects.

MultiTag objects
----------------
- Rules for `Tag objects`_ apply.
- Positons can be one- or two-dimensional:
  - If the Positions array is one-dimensional, the referenced DataArray objects must also be one-dimensional.
  - If the Positions array is two-dimensional, the length of each element (the second dimension) must match the number of dimensions of all referenced DataArray objects.
- If the MultiTag contains Extents, the shape of the Extents array must match the shape of the Positions array.

Feature objects
---------------
- Rules for `Entities`_ apply (excluding `name` and `type`).
- Data reference must be set: HDF5 group `data`.
- LinkType must be set and have a valid value: HDF5 attribute `link_type`.

Source objects
--------------
- Rules for `Entities`_ apply.

Section objects
---------------
- Rules for `Entities`_ apply.

Property objects
~~~~~~~~~~~~~~~~
- Rules for `Entities`_ apply (excluding `type`).

Soft rules
==========

File objects
------------
- Version should be set: HDF5 attribute `version`.
- Format should be set: HDF5 attribute `format`.

DataArray objects
-----------------
- Unit should be SI or composite of SI units.
- If Polynomial Coefficients are set, the expansion origin should also be set and vice versa.

Dimension objects
~~~~~~~~~~~~~~~~~
- SampledDimension rules:
  - If Offset is set a valid unit must also be set: HDF5 attribute `offset`.

Property objects
----------------
- If Value is set, it should also have a Unit: HDF5 attribute `unit`.
