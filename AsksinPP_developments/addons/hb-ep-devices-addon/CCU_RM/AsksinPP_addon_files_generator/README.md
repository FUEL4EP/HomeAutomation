 
# Groovy generator script [AsksinPP_addon_generator.groovy](./AsksinPP_addon_generator.groovy) for automatically generate device specific install and uninstall scripts

- creates install and uninstall scripts required in [../src/addon/](../src/addon/)

- The following software installations are prereqisites:

	+ install [groovy](https://groovy-lang.org/), a multi-faceted language for the Java platform, on your computer
	+ install [XML Copy Editor](https://xml-copy-editor.sourceforge.io/) on your computer
	+ both groovy and XML Copy Editor are free software and available for Windows, Mac, and Linux. Please choose an appropriate source for these software based on your system setup.
- Please follow the following steps to create the addon install and uninstall scripts
	+ create if not yet exists a new working directory '[_work](./_work)' 
	+ create if not yet exists a subdirectory '[<Device_name: here HB-UNI-Sensor-EXAMPLE>](./_work/HB-UNI-Sensor-EXAMPLE)' for your device for which you want to generate the addon install and uninstall scripts
	+ copy the [example XML control](_work/HB-UNI-Sensor-EXAMPLE/HB-UNI-Sensor-EXAMPLE_addon_control.xml) file to this working directory '[<Device_name: here HB-UNI-Sensor-EXAMPLE>](./_work/HB-UNI-Sensor-EXAMPLE)
	+ copy the [XML Schema file](_work/AsksinPP_addon_control_file.xsd) file to this working directory '[<Device_name: here HB-UNI-Sensor-EXAMPLE>](./_work/HB-UNI-Sensor-EXAMPLE)
	+ copy the [example XML control](_work/HB-UNI-Sensor-EXAMPLE/HB-UNI-Sensor-EXAMPLE_addon_control.xml) in your new device's working directory to your new sensors XML control file <e.g. HB-UNI-Sensor-XXX-YYYY_addon_control.xml>. File extension needs to be '.xml'.
	+ change to your working directory '[<Device_name: here HB-UNI-Sensor-EXAMPLE>](./_work/HB-UNI-Sensor-EXAMPLE)
	+ edit your new sensors XML control file <e.g. HB-UNI-Sensor-XXX-YYYY_addon_control.xml> with the XML Copy Editor.
	+ you should get [this](./Images/HB-UNI-Sensor-EXAMPLE_addon_control.xml.png) window.
	+ now edit in XML Copy Editor the XML elements according to your needs:
		* add or delete data_points according to your new device's needs
		* the element device_class can be chosen arbitrarily for each datapoint. If you want to skip it, enter 'SKIP'.
		* the element device_description is automatically HTML4 encoded by the generator script, so no need to take care e.g. of Umlauts here
		* the elements translated_string are automatically URL encoded by the generator script, so no need to take care e.g. of Umlauts here
		* after having done all changes, please check your XML:
			- click on the blue checkmark of the XML copy menu to check whether your XML is well-formed
			- click on the green checkmark of the XML copy menu to check whether your XML is valid. If it is valid (see message at the bottom) save it.
			
	+ now invoke the groovy generator script in a terminal window as follows:
> 		groovy <path_to_groovy_script>/AsksinPP_addon_generator.groovy
	+ next you will be asked for the file name of the generated XML control file, please enter it e.g. <HB-UNI-Sensor-XXX-YYYY_addon_control.xml> .
	+ the generator script will create an install and an uninstall script in the working directory
	+ check the created install and an uninstall script before you yuse it within an AsksinPP addon
