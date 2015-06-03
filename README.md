##Kid Sudoku Editor##

###Concept & Constraint

* **Grid** is made up by cells, each cell corresponding to one number. A *grid* 's size could be 1x2, 1x3, 1x4, 2x2, 2x3, 2x4, 3x3 cells.

* **SudokuBox** is made up by *grids*.

* **Stage** corresponding to game scene, makes the game continuable.

* **Campaign** is made up by stages. Each campaign has one unique indentifer, use *uuid* as this unique identify.


* **UUID** is a universally unique identifier that is generated using random numbers. It's treated as a packet made up by four unsigned integers. UUID would be generated by server, that means local campaign has no need to has one uuid, only those campaign shared on internet need to has one.

* **Packed campaign file** is for editor. It would be unpacked before it could be used by kidsudoku game.

###Why the file be packed
The resouce of a campaign or even a stage could be specified. Normally the resource files include plist file(s) and png file(s), if just record the file path, we would get errors on opening this campaign witout there resource files, so we must pack these resource files with the campaign file.

At first I want to select *ZIP* as archive file format, but it's not well supported by Qt, on the other hand, the binary data would not be compressed much, so I just append the file content at the end of the campaign data.

###About share the campaigns on internet
I want to make the game extendible. Any one could make campaigns for it. Then to the game, the puzzle stages and resource files would be two kinds of extendible resources, and the resource files should be reusable.

So, I got tow idea. The first one, there should be a server, publish campaign packed files. These campaign files would be unpacked by the server, and the resources of this campaign (if it has specified) would be shared on this server. 

The other one, make the game flexible to using resources just like theme. So people just need to edit the puzzle or edit the art.

I choose the first one at now, because this one make it possible to change art resources in a campaign. Maybe I would change my mind in the future.

###Data structure packing format
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
|len|uint_32|4|length of StageData exclude this field|
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

* ResourceFileData

	|   name | type | length | comment |
| --------- | -------:| ---------:|:-------------- |
|len|uint_32|4|lenght of this packet exclude this field|
|name|string||file name
|content_length|uint_32|
|content|array||content of file.

* Packed campaign file format

	|   name | type | length | comment |
| --------- | -------:| ---------:|:-------------- |
|campaign_data_len|uint_32|4| length of following campaign data.
|campaign_data|CampaignData|||
|count|uint_32||count of resource files|
|files|array||array of ResourceFileData



