# Digital Twin Frontend Design

This document contains the overall design scheme of Lab Digital Twin’s frontend.



## To-Do List

This list is in descending urgency. A more concise version is in the project slides.

- [ ] ==Take `const updatelist` out of the iteration.==

### Urgent Fixs

These include frequent errors, bugs that are easy to fix, and serious malfunctions.

- [ ] ==Change the reloading icon’s position.== The icon is currently manually placed in a wrong position using pixels as unit in the CSS code. Change the CSS code to place it in a correct position, ideally using vw/vh or percentage instead of pixels as unit.
- [ ] ==Do inventory list without aligning SKU to row number.== Currently the inventory list works with the Google Sheets API through reading the SKU to get the row number information. This limits the usefulness of SKUs and is very unstable. Find a way to read the row numbers of and switch to a more informative SKU naming system.
- [ ] ==Change SKU from string to integer.== When using Add/Edit now the SKUs will be inserted into the sheet as strings. Find a way to deliver them as integers.
- [ ] ==Make list item clickable instead of only text being clickable.== Currently clicking on an item to select it and show info only works if the name text of that item is clicked. This is because the function call is bound to the text as a link. Try to bound the function call to the item div as an onclick event.
- [ ] ==Fix the issue of activating Add and Edit at the same time.== Currently clicking on either Add or Edit will show the corresponding operations area and the info display in turns. However, activating both of them at the same time will cause an error where deactivating any one of them would call back the info display. This issue should be fixed, and only when both of them are deactivated can the info display be shown.
- [ ] ==Issue of multiple points at same location.== When search results are equal to or less than 5, all results’ locations will be drawn on the floor plan. However if two points show at the same location their texts will overlap. Find a way to prevent texts from overlapping. (maybe use a variable to track how many points are at the same location and set a position offset to the text accordingly)
- [ ] ==Search bar error fix.== Using the “X” button or typing enter in the search bar would cause errors. These might be caused by using onkeyup event with the search bar element. Fix these errors to make search bar behave normally.
- [ ] ==Optimize everything to fit any ratio.== See UI-Screen/Page Ratio for more details.



### New Functions

These are new functions building on exisiting functions/UI.

- [ ] ==Logic of adding new places/user. Delete function for location/user and its UI.== There is already logic in the code where selecting the “… New” option will call an empty function. Modify the empty function to make it actually do the adding. After that, add the new logic for deleting, probably with a button aside the dropdown selection.
- [ ] ==Make location entry 2-stepped. Full location list function.== The location list display function is already done, currently commented out in the code with example data only. Modify the Google Sheets backend to implement the location list function. After that is done, modify all location entry element to be 2-stepped, adding a text entry box after the dropdown selection to let user enter the “y-value” of the location.
- [ ] ==Show y-axis location with the dots.== After “y-values” of the locations is implemented into the backend, show them with the current location dots as some text aside. This uses canvas.
- [ ] ==New category search.== In a legacy version there was a basic category search, where typing in the full category name and a “;” will show all items under that category, as well as adding the highlighted category name in a pair of brackets behind all item names. This was deleted after implementing the Google Sheets backend. Re-do this function and improve it.
- [ ] ==Add a button for magnetic data calculation.== The magnetic datas might need some calculation to show more useful information. Add a button for that and show the calculated datas in a new place.
- [ ] ==Download function.== As the History operations interface locks the plots to a certain duration in time, the download function exports that exact snapshot of plots and allow the user to download it. The download button is already in place.



### Future Optimizations

These are optimizations to be done when the project reaches a relatively steady state.

- [ ] ==Improve markdown function.== Add more tags for recognition.
- [ ] ==Try using extend with Plotly.== There is a .extend function in the Plotly API. This function might improve the overall plotting performance, try it on a stable version.
- [ ] ==Barcode generating/printing and its interface. See Yufeng’s sample code.== Turning a selected equipment/location/user into a barcode. Yufeng already has sample code in the functional_modules folder.
- [ ] ==Sort list. Redo dropdown list, implement through object list. Make category keywords.== Sort the inventory list by category. There was a legacy dropdown list function, re-implement that. Confirm on the way to separate category keywords.
- [ ] ==Show text when hovering over certain locations. Use clicking to implement location selection.== Detect the mouse location and show location details when hovered over. Make the location selected when clicked.
- [ ] ==Optimize searching function.== Improve the searching function.
- [ ] ==Animation.== Add some animation to the webpage.
- [ ] ==Implement wave and bubble UI.== Switch the current UI design to a more polished one.
- [ ] ==Switch the floorplans to the 3D models, explore texture, opacity etc.== In a legacy version the floorplans are 3D models using three.js. Re-do that and find a way to manipulate the models’ texture and opacity.



## UI

### Overall Layout

The main body of the webpage is separated into three columns: The floorplan column, the equipment list column, and the environmental plotting column.

- The floorplan column: this part contains the lab selector and the user selector. The lab selector controls which floorplan is displayed underneath. Between the selectors and the floorplan there is a place holder icon to display the current user of some equipment.
- The equipment list column: this part contains the searchbar, the equipment list itself, and the operations area. The operations area is by default the info display and take/return interface. By clicking the “Add” or “Edit” buttons, the operations area will be changed into respective modes, and changes back on a second click.
- The environmental plotting column: this part contains the timescale buttons, the plots themselves, and the history mode area. The history operation interface is activated by clicking the “History” button, and deactivated by clicking the “Present” button(previously the “History” button).

Aside from the three columns, there’s also a quick access link in the top right corner of the webpage. This link acts as a quick access to the lab’s main website.

The header and the footer are currently not in use. The header can be used as a title area or a navigation bar. The footer is coded to be sticky to the bottem of the page regardless of any amount of scrolling, and can be used as a copyright/credit display area.



### Button Style

There are mainly two types of buttons on this webpage: on-click buttons and status buttons.

- On-click buttons: these buttons include confirm buttons, delete buttons, take/return buttons, and the download button.
- Status buttons: these buttons include add/edit buttons, timescale selection buttons, and the history button.

On-click buttons have a pair of hover color and activation color. The two colors are in similar tones but the hover color will be lighter.

Status buttons have the unactivated state and the activated state, each having their own pair of hover color and activation color. All status buttons have two sets of color styling in the CSS code.

All buttons are default to beige color, in the activated state status buttons will have a different color to represent their current state.



### Screen/Page Ratio

Currently most elements are defined by percentage or vw/vh, and in the CSS file there is code dedicated to make the column width respond to the page width. The point drawing mechanism on the canvas uses a percentage system so it’s not affected by page size change. With in the plotting area each single plot adjust themselves autometically.

However, the canvas size is predetermined in pixels in the HTML code, and the size of the whole plotting area is decided in pixels in the JS code. These two elements still do not respond to page size change and should be modified to do so. Especially the canvas element, since it would cover up a part of the equipment list under certain page ratios. The loading icon is also currently manually placed.



### Other Elements

- The loading icon: to cover the response time of plotting, needs to change place.
- The scroll bar: has logic in HTML code to only show when hovered, has custom coloring.
- The selection element: has custom coloring.



## Equipment List

### Main Logic

The “refresh list” button will get data from the Google Sheets backend. After reveiving the data, a map from each object’s SKU to the object itself is created. For each object an HTML code segment is also added to the search result area. These code segments include calls to the objSelect function, which changes the currObj variable accordingly. This is the way to inform various diaplay areas of what information to show.



### Search Engine

Currently uses a simple keyword match. Will try to show points on the floorplan when the search results are less than or equal to 5.



### Add/Edit operations

First retrives data from the operations areas. If any of the critical datas (name and SKU) are empty, than clicking “confirm” will have no effect. After “confirm” is successfully clicked, the data will be handed to the Google Sheets backend and the operations area will close. Google account login will be triggered twice, once when the Add/Edit buttons are clicked and once “confirm” or “delete” is clicked.

Button appearance control code is separated from button behavior manipulation code.



### Scroll Bar

There is a logic within the HTML code that only shows the scroll bar of the equipment list when hovered over. The scroll bar is customly styled.



### Point Mechanism

When an equipment is clicked on or when the search results are less than a certain number, points representing these equipments’ location will show on the floorplan. The points are implemented through the canvas element. There is a pointMap that maps location names to x/y ratios of their physical locations on the floorplan.



## Environmental Plot

### Receiving the Data

Environmental data is delivered through WebSocket by sending in requests. The data’s range and end time can be changed.(will bw covered in later parts) Default range is 1 hour and default end time is current time.

An array should be responsible for storing the data and the plotting information to send to Plotly. This includes:

- xaxis/yaxis: This determines which graph a certain data stream goes into. The x/y should be in a pair.
- legendgroup: This determins the data stream’s legeng group. Datas of the same room should be in the same legend group.
- name: The room name for a certain data stream. Datas of the same room should have same names.
- mode/type: decides how datas shall be represented.
- showlegend: This attribute is defaultly set to true, and by setting it to true the corresponding legends’ names and colors will be show aside the plots. Because of this, only one data stream from each room should be set to true and all other shouls be set to false.

After the data is being processed, they’ll go into corresponding spaces in this array.



### Processing the Data

As different node send datas at different times, to make the plot look normal we need to deal with the small differences within the timestamps and align the plots to a same moment. We will align all datas to the first stream of data received from a request.

Then we just have to choose the right data to push into the correct place within the plotting data array. This is achieved through the “placement” variable, which grows by the number of rooms after each iteration.

The updatelist confirms what types datas to plot and thus abandoning all other datas. Note that the data types within the updatelist have to align with the plotting data array and the layout.

Some raw datas need processing and this shall be done before the data is pushed into the plotting array.



### Plotting the Data

The actually plotting is done by Plotly. However, we have to specify the plotting layout:

- legend: Decides the appearance, location, and orientation of the legend display.
- colorway: Decides the color for each data stream.
- grid: Decides the layout for multiple plots.
- autosize/width/height/margin: Decides the size of the whole plotting area.
- annotations: Used to show the data types.
- xaxis/yaxis: Decides the styles of x and y axises. Notice that xaxis1 and xaxis2 are different from other x axises because they are the bottom two x axises. The units for different data streams are displayed on the y axises.



### Setting the Interval

The time duration buttons changes the range of plotting, this works for both normal mode and history mode.



### Accessing the History

When history mode is on, the real-time plotting is autometically stopped. The user can then set a certain end time for the plots through the date-time selector. Clicking the “confirm” button afterwards will call Plotly to plot a stationary plot of the selected range and end time.

Button appearance control code is not separated from button behavior manipulation code.



## Misc.

### Button Behavior

All buttons call functions through the “onclick” attribute.



### Markdown Parser

The markdown reader was first built upon the purpose to include hyperlinks to outer product pages for the equipments. It was later decided that a fully functional markdown recognition function would be useful.

The markdown reader parses incoming text with regex and turns it from markdown tags into corresponding HTML tags.



### Lab Selection

Labs can be paired with their respective floor plans through the labsw function.



---

For any errors or questions contact [daniel_tan@berkeley.edu](daniel_tan@berkeley.edu).
