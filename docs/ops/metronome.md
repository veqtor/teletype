##Metronome

An internal metronome executes the M script at a specified rate (in ms). By default the metronome is enabled (M.ACT 1) and set to 1000ms (M 1000). The metro can be set as fast as 25ms (M 25). An additional M! op allows for setting the metronome to experimental rates which when used with a high volume of i2c commands in the M scripts can cause teletype stability issues.

Access the M script directly with `alt-<F10>` or run the script once using `<F10>`.
