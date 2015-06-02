##Kid Sudoku Editor##

###Concept & Constraint

* **Grid** is made up by cells, each cell corresponding to one number. A *grid* 's size could be 1x2, 1x3, 1x4, 2x2, 2x3, 2x4, 3x3 cells.

* **SudokuBox** is made up by *grids*.

* **Stage** corresponding to game scene, makes the game continuable.

* **Campaign** is made up by stages. Each campaign has one unique indentifer, use *uuid* as this unique identify.


* **UUID** is a universally unique identifier that is generated using random numbers. It's treated as a packet made up by four unsigned integers.


---

###Campaign file format
* UUID

	|   name | type | length | comment |
| --------- | -------:| ---------:|:-------------- |
|id4|uint_32|4
|id3|uint_32|4
|id2|uint_32|4
|id1|uint_32|4

* string

	|   name | type | length | comment |
| --------- | -------:| ---------:|:-------------- |
|str_len|uint_32|4|
|content|array||char array, length indicated by *str_len*

* StageData

	|   name | type | length | comment |
| --------- | -------:| ---------:|:-------------- |
|id|uint_32|4|indicate this stage, would be used in game's progress data.
|res|string||if this stage use another resouce differet to *default_res* of CampaignData, fill this field.
|rows_per_grid|uint_8|1
|cols_per_grid|uint8|1
|grids_in_row|uint8|1
|grids_in_col|uint8|1
|known_cells_count|uint16|2|count of cells that has initialized with a number in this puzzle.
|know_cells|array|dynamic|uint_16 array, each value is made up by two parts: low 8-bit indicates position of this cell and high 8-bit indicate its number value.



* CampaignData

	|   name | type | length | comment |
| --------- | -------:| ---------:|:-------------- |
|major_version|uint_16|2|
|minor_version|uint_16|2
|id|UUID|16|
|default_res|string|| content.
|stage_count|uint_32|4| count of stage data in this campaign
|stage_offset|array|dynamic|uint_32 array, each value indicates the offset of the stage data stored in the campaign file.
|stage_data|array|dynamic|StageData array


