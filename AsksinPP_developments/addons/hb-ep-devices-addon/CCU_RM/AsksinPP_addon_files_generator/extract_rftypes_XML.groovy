#!/usr/bin/env groovy

/* # latest checked occu version: https://github.com/eq-3/occu/commit/a420be85d65df323a6c94efc5b132dbe9a2e3f24  version 3.69.7 */

package extract_rftypes_XML
/**
* A simple extractor for translation candidates of a rftypes XML file.
*/ 

import java.nio.file.FileSystems
import java.nio.file.Path
import java.io.File
import javax.xml.xpath.*
import javax.xml.parsers.DocumentBuilderFactory
import org.w3c.dom.*
import javax.xml.parsers.*
import java.io.*
import java.lang.String
import groovy.xml.XmlUtil
import groovy.util.Node
import groovy.json.JsonSlurper
import groovy.xml.MarkupBuilder
import groovy.xml.*


class extract_rftypes_XML {

    static VersionString                       = "0.14"
    static Map_Storage_Identifier              = "extract_rftypes_XML map storage file, please don't touch!\n\n"
    static Addon_translation_archive_file      = "Addon_extract_rftypes_XML_translation_archive.db"
    static Addon_control_XML_file              = "AsksinPP_addon_generator_control_file.xml"
    static Occu_translation_archive_file       = "EQ3_occu_extract_rftypes_translation_archive.db"
    static Addon_name                          = "My_Addon"
    static Addon_control_XML_file_translation  = "AsksinPP_addon_generator_control_file_translation_source.xml"
    static Easymode_tcl_directory              = "../src/addon/www/config/easymodes"  // relative path of easymode tcl files
    
    static def parse_webui_js(file_name){
 
        println "\n.. reading '${file_name}' .."
        def inputFile = new File(file_name)
        
        String required_match_regex = /^CHANNELCHOOSER_JST[\s\S]*return service_count;\n\};\n\n$/

        if ( !( inputFile.text ==~ required_match_regex) )
        {
          println "\ninput file ’${file_name}' has wrong syntax!\n"
          println "Aborting .."
          System.exit(0)
        }
        
        def map = [:]
        
        //split into lines by linefeed
        def lines = inputFile.text.split(/\n/)
        //check for lines starting with 'elvST'
        lines.each {
          if ( it =~ /^elvST\[/ ) {
            if ( it.size() > 2 ) {
              //regex for extracting elements separated by "'"
              def m_pattern   = /([^']*)'([^']*)'([^']*)'\s*([^']*)';$/

              def m = (it =~ m_pattern)
              //println m[0][2]
              //println m[0][4]
              def p = m[0][4].replaceAll(/\$/,"")
              p = p.replaceAll(/\{/,"")
              p = p.replaceAll(/\}/,"")
              //println p
              def q = p.split(/ /)
              //println q
              map[m[0][2]] = q
            }
          }
        }
	return map 
    } 
    
    static def parseStringTable(file_name){
 
        println "\n.. reading '${file_name}' .."
        def inputFile = new File(file_name)
        
        String required_match_regex = /^\nABSOLUTE_ANGLE[\s\S]*\{lblOverflow\}\n$/

        if ( !( inputFile.text ==~ required_match_regex) )
        {
          println "\ninput file ’${file_name}' has wrong syntax!\n"
          println "Aborting .."
          System.exit(0)
        }
        
        def map = [:]
        
        //split into lines by linefeed
        def lines = inputFile.text.split(/\n/)
        //split into words; second word onwards need to start with a '$'
        for (String s in lines) {
          String[] w  = s.split(/\s+\$/)
          // remove curly brackets and empty lines
          if ( w.length > 1 ) {
            def wc = w.collect {
             it = it.trim().replaceAll("[\\{\\}:]","")
            }
            //assign to map
            map[wc[0]]=wc[1..-1]
          }
        }
	return map 
    }
    
     static def parseJson(file_name){
 
        println "\n.. reading '${file_name}' .."
        def inputFile = new File(file_name)
        
        String required_match_regex = /^jQuery\.extend\(true\,langJSON\,\ \{\n\ \ \"de\"\ \:\ [\s\S]*langJSON.de.stringTableValveStateErrorPosition;$/


        if ( !( inputFile.text ==~ required_match_regex) )
        {
          println "\ninput file ’${file_name}' has wrong syntax!\n"
          println "Aborting .."
          System.exit(0)
        }


        
        //remove jquery.extend string at the beginning
        def json_text = inputFile.text - ~/^jQuery\.extend\(true\,langJSON\,\ \{\n\ \ \"de\"\ \:\ /
        
        //remove jquery.extend string at the end
        json_text =  json_text - ~/\)\;\n$/
        
        //remove line HTML line breaks
        json_text =  json_text.replaceAll(/\"\s*\+\s*\"/, "")
        json_text =  json_text.replaceAll(/\<br\/\>/, " ")
        
        //println json_text
        //now we have a string in json syntax and can parse it
        
	def lazyMap = new groovy.json.JsonSlurper().parseText(json_text)
	def map = [:]
	for ( prop in lazyMap ) {
    	map[prop.key] = prop.value
	}
	return map
    }
    
     static def check_file(String file_name) {
       
        def file = new File(file_name)

        if ( ! file.exists() ){
          println "The file '${file_name}' is NOT existing"
          println "Aborting .."
          System.exit(0)
        }

        if ( ! file.isFile() ){
          println "The file '${file_name}' is NOT a regular file"
          println "Aborting .."
          System.exit(0)
        }

        if ( ! file.canRead() ){
          println "The file '${file_name}' is NOT readable"
          println "Aborting .."
          System.exit(0)
        }
        
        return file_name
    }
    
    static def check_existance_of_file(String file_name) {
       
        def file = new File(file_name)
        
        def exists = true

        if ( ! file.exists() ){
          println "\nThe file '${file_name}' is NOT existing"
          exists = false
        }
        else {
          if ( ! file.isFile() ){
            println "\nThe file '${file_name}' is NOT a regular file"
            exists = false
          }
          else {
            if ( ! file.canRead() ){
              println "\nThe file '${file_name}' is NOT readable"
              exists = false
            }
          }
        }
        
        return exists
    }

    
    static def check_control_XML_file_name(String rftypes_XML_file_name) {
       
        def file = new File(rftypes_XML_file_name)

        if ( ! file.exists() ){
        println "The file '${rftypes_XML_file_name}' is NOT existing"
        println "Aborting .."
        System.exit(0)
        }

        if ( ! file.isFile() ){
        println "The file '${rftypes_XML_file_name}' is NOT a regular file"
        println "Aborting .."
        System.exit(0)
        }

        if ( ! file.canRead() ){
        println "The file '${rftypes_XML_file_name}' is NOT readable"
        println "Aborting .."
        System.exit(0)
        }

        def rftypes_XML_file_name_extension = rftypes_XML_file_name.lastIndexOf('.').with {it != -1 ? rftypes_XML_file_name.substring(it+1):''}

        if ( rftypes_XML_file_name_extension != 'xml' ) {
        println "The extension of '${rftypes_XML_file_name}' is not 'xml': '${rftypes_XML_file_name_extension}'"
        println "Aborting .."
        System.exit(0)
        }

        try {
        File fXmlFile = new File(rftypes_XML_file_name)
        DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance()
        DocumentBuilder dBuilder = dbFactory.newDocumentBuilder()
        Document doc = dBuilder.parse(fXmlFile)
        } catch(Exception ex) {
            println (" ")
            println("E R R O R : specified rftypes XML file '${rftypes_XML_file_name}' is not a well-formed XML file")
            println (" ")
            println "Aborting .."
            /*throw new RuntimeException("The specified rftypes XML file '${rftypes_XML_file_name}' is not a well-formed XML file", ex)*/
            System.exit(0)
        } 
        
        return rftypes_XML_file_name
    }

    static def get_control_XML_file_name() {
    
        print "Please input the file name of the AsksinPP rftypes XML file : "
        def rftypes_XML_file_name = System.in.newReader().readLine()
        
        def checked_rftypes_XML_file_name = check_control_XML_file_name(rftypes_XML_file_name)
        
        return checked_rftypes_XML_file_name
        
    } 
    
    static def check_if_key_already_exists( key0, key, Map translation_map, Map webui_js_map, Map StringTable_de_map, Map translate_lang_stringtable_map, parameters ) {
    
        def key_already_exists = false

        if ( webui_js_map.containsKey(key) ) {
            if ( parameters.verbose ) {
              println "=> '${key}' already defined in 'webui.js'"
            }
            key_already_exists = true
        }
        else {
          if ( StringTable_de_map.containsKey(key) )  {
            if ( parameters.verbose ) {
              println "=> '${key}' already defined in 'stringtable_de.txt'"
            }
            key_already_exists = true
          }
          else {
            if ( translation_map[key0].containsKey(key) ) {
              def my_stringtable_key = translation_map[key0][key]."stringTable_name"
              if ( translate_lang_stringtable_map.containsKey(my_stringtable_key) ) {
                if ( parameters.verbose ) {
                  println "=> '${key}' already defined in 'translate.lang.stringtable.js'"
                }
                key_already_exists = true
              }
            }
          }
        }
          
        return key_already_exists
    
    }
    
    
    static def remove_duplicates_from_translation_map(Map translation_map, Map webui_js_map, Map StringTable_de_map, Map translate_lang_stringtable_map, parameters) {
    
        def keys_to_be_deleted
        
    
        translation_map.each{ key0, val0 ->
        
           //println ".. working on primary key '" + key0 + "' .."
           keys_to_be_deleted = []
        
           val0.each { key, var  ->
        
            
            def key_str = ""
            
            key_str = key
            
            def key_already_exists = false
            
            //key_already_exists |= check_if_key_already_exists ( key0, key, translation_map, webui_js_map, StringTable_de_map, translate_lang_stringtable_map, parameters)
            
            if ( var["type"] == "channel" ) {
              key_str = var["channel_type"] + "|" + key
              key_already_exists |= check_if_key_already_exists ( key0, key_str, translation_map, webui_js_map, StringTable_de_map, translate_lang_stringtable_map, parameters)
            } else {
              if ( var["type"] == "param_defs" ) {
              if (  var["channel"] != "" ) {
                key_str = var["channel"] + "|" + key
              }
              else {
               key_str = "KEY|" + key
              }
              key_already_exists |= check_if_key_already_exists ( key0, key_str, translation_map, webui_js_map, StringTable_de_map, translate_lang_stringtable_map, parameters)
              }
            }
            
            if ( key_already_exists ) {
              keys_to_be_deleted.add(key)
            }
          }
          
          //println keys_to_be_deleted
          // delete duplicated keys in translation_map[key0]: channel, paramset
          if ( keys_to_be_deleted.size() > 0 ) {
            println ".. ${keys_to_be_deleted.size()} duplicated entries were removed from primary key '${key0}' .."
            //println ".. removed keys : " + keys_to_be_deleted
          }
          
          // remove duplicated entries in translation_map
          keys_to_be_deleted.each {
            translation_map[key0].remove(it)
          }
          
        }
        return translation_map
    }
    
    static def remove_duplicates_from_map(  Map map_target,  Map map_reference, Map parameters, String map_name ) {
    
        def keys_to_be_deleted = []
        
        println ".. working on '${map_name}'"
        
        
        map_target.each { key0, val0 ->
        
          println ".. working on primary key '${key0}' .."
          //println map_reference[key0]
          if ( map_reference.containsKey(key0) ) {
            val0.each { key, var  ->
                println ".. working on secondary key '${key}' .."
                if ( map_reference[key0].containsKey(key) ) {
                  //println "contained : " + map_reference[key0].containsKey(key)
                  if ( parameters.verbose ) {
                      println "=> '${key}' already defined in EQ3's occu '${map_name}'"
                  }
                  keys_to_be_deleted.add(key)
                  //println keys_to_be_deleted
                }
            }
          }
          // remove duplicated entries in map_target
          keys_to_be_deleted.each {
            map_target[key0].remove(it)
          }
          
          println ".. ${keys_to_be_deleted.size()} duplicated entries were removed from '${map_name}' and primary key '${key0}' .."
          
        }
        return map_target
    }
    
    
    static def remove_duplicates_from_addon_map ( Map addon_db_map, Map reference_db_map, Map parameters) {
    
        if ( parameters.verbose ) {
          println "\n\n.. before removing duplicates the Addon master_parameter_translation_map has " + get_size_of_nested_map_depth_2(addon_db_map.master_parameters) + " entries"
          println ".. before removing duplicates the Addon data_points_translation_map has " + get_size_of_nested_map_depth_2(addon_db_map.datapoints) + " entries"
          println ".. before removing duplicates the Addon paramset_translation_map has " + get_size_of_nested_map_depth_2(addon_db_map.paramsets) + " entries"
          
          println "\n\n.. before removing duplicates the Reference master_parameter_translation_map has " + get_size_of_nested_map_depth_2(reference_db_map.master_parameters) + " entries"
          println ".. before removing duplicates the Reference data_points_translation_map has " + get_size_of_nested_map_depth_2(reference_db_map.datapoints) + " entries"
          println ".. before removing duplicates the Reference paramset_translation_map has " + get_size_of_nested_map_depth_2(reference_db_map.paramsets) + " entries\n\n"
        }

        addon_db_map.master_parameters = remove_duplicates_from_map( addon_db_map.master_parameters, reference_db_map.master_parameters, parameters, "Master parameters map")
        addon_db_map.datapoints = remove_duplicates_from_map( addon_db_map.datapoints, reference_db_map.datapoints, parameters, "Datapoints map")
        addon_db_map.paramsets = remove_duplicates_from_map( addon_db_map.paramsets, reference_db_map.paramsets, parameters, "Keys map")
        
        return addon_db_map

    }
    
    static def write_control_xml_file (String XML_text , String file_name) {
    
      File file = new File(file_name)
      
      if ( file.exists() ){
        println "\n\nW A R N I N G: The file '${file_name}' is already existing!"
        println "File '${file_name}' will be renamed to '" + file_name + ".bck_" + get_current_date_time() + "'"
        file.renameTo file_name + ".bck_" + get_current_date_time()
        file = new File(file_name)
      }
      
      file.write(XML_text)
    
    }
    
    static def save_maps_to_file (Map store_map, Map parameters, String file_name) {  
    
      File file = new File(file_name)
      
      if ( ! parameters.no_addon_db_backup ) {
        if ( file.exists() ){
          println "\n\nW A R N I N G: The file '${file_name}' is already existing!"
          println "File '${file_name}' will be renamed to '" + file_name + "." + get_current_date_time()  + ".bck'"
          file.renameTo(file_name + ".bck_" + get_current_date_time())
          file = new File(file_name)
        }
      }
      
      def string_containing_serialized_store_map = store_map.inspect()
    
      file.write(Map_Storage_Identifier)
      file.append("created by '" + get_groovy_script_name() + "' " + VersionString + "\n")
      file.append("creation date : " + get_current_date_time() + "\n")
      file.append("creator : " + System.getProperty("user.name") + "\n\n")
        
      file.append(string_containing_serialized_store_map)
    
    }
    
    static def extract_string_translations_from_previous_xml_control_file( String XML_file_name ) {

      def string_translation_maps    = [:]
      
      def string_translation_de_map  = [:]
      def string_translation_en_map  = [:]
      

      
      
      def file = new File(XML_file_name)
      def my_stringtable_entry
      
      if ( file.exists() ){
        if ( file.isFile() ){
           if ( file.canRead() ){
            println "\n\n.. reading the previous XML control file '${XML_file_name}' and extracting it's translations ..\n"
            def xml = new XmlSlurper().parse(file)

            xml.children().each {
              if (it.name() == "paramset" ) {
                it.children().each {
                  if (it.name() == "parameter" ) {
                    it.children().each {
                      if ( it.name() == "stringtable_identifier" ) {
                        my_stringtable_entry = it.toString()
                      }
                      if ( it.name() == "translation_de" ) {
                        string_translation_de_map[my_stringtable_entry]=it.toString()
                      }
                      if ( it.name() == "translation_en" ) {
                        string_translation_en_map[my_stringtable_entry]=it.toString()
                      }
                    }
                  }
                }
              }
              if (it.name() == "channels" ) {
                it.children().each {
                  if (it.name() == "channel" ) {
                    //println it.@type
                    it.children().each {
                      if (it.name() == "parameter" ) {
                        it.children().each {
                          if ( it.name() == "stringtable_identifier" ) {
                            my_stringtable_entry = it.toString()
                          }
                          if ( it.name() == "translation_de" ) {
                            string_translation_de_map[my_stringtable_entry]=it.toString()
                          }
                          if ( it.name() == "translation_en" ) {
                            string_translation_en_map[my_stringtable_entry]=it.toString()
                          }
                        }
                      }
                    }
                  }
                }
              }
              if (it.name() == "param_defs" ) {
                it.children().each {
                  if (it.name() == "parameter" ) {
                    it.children().each {
                      if ( it.name() == "stringtable_identifier" ) {
                        my_stringtable_entry = it.toString()
                      }
                      if ( it.name() == "translation_de" ) {
                        string_translation_de_map[my_stringtable_entry]=it.toString()
                      }
                      if ( it.name() == "translation_en" ) {
                        string_translation_en_map[my_stringtable_entry]=it.toString()
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      else {
        println "\n\n.. a previous XML control file '${XML_file_name}' doesn't exist and therefore no translations can be extracted! ..\n\n"
      }
      
      string_translation_maps["de"] = string_translation_de_map
      string_translation_maps["en"] = string_translation_en_map
      
      //println string_translation_de_map
      //println string_translation_en_map
      
      println ".. the extracted string_translation_de_map has " + string_translation_maps["de"].size() + " entries"
      println ".. the extracted string_translation_en_map has " + string_translation_maps["en"].size() + " entries"
    
      return string_translation_maps
    }
    
    static def extract_device_descriptions_from_previous_xml_control_file( String XML_file_name ) {
    
      def device_descriptions_map   = [:]
      
      def file = new File(XML_file_name)
      def my_device_entry
      
      if ( file.exists() ){
        if ( file.isFile() ){
           if ( file.canRead() ){
            println "\n\n.. reading the previous XML control file '${XML_file_name}' and extracting it's device descriptions ..\n"
            def xml = new XmlSlurper().parse(file)

            xml.children().each {
              if (it.name() == "devices" ) {
                it.children().each {
                  if (it.name() == "device" ) {
                    my_device_entry = it.@id.toString()
                    it.children().each {
                      if ( it.name() == "device_description" ) {
                        device_descriptions_map[my_device_entry] = it.toString()
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      else {
        println "\n\n.. a previous XML control file '${XML_file_name}' doesn't exist and therefore no device descriptions can be extracted! ..\n\n"
      }
      
      println ".. the extracted device_descriptions_map has " + device_descriptions_map.size() + " entries"
      
      //println device_descriptions_map
    
      return device_descriptions_map
    }
    
    static def read_maps_from_file (String file_name) {
    
      def checked_file_name = check_file(file_name)
      
      String required_match_regex = /^extract_rftypes_XML map storage file, please don't touch!\n\ncreated by '/

      File restoreFile = new File(checked_file_name)
      
      def string_containing_serialized_map = restoreFile.getText('UTF-8')
      
      if ( !( string_containing_serialized_map =~ required_match_regex) )
      {
          println "\ninput file ’${checked_file_name}' has wrong syntax!\n"
          println "Aborting .."
          System.exit(0)
      }
      
      string_containing_serialized_map = string_containing_serialized_map - ~/^.*\n\ncreated by.*\ncreation date.*\ncreator.*\n\n/
      
      //println "Serialized"
      //println string_containing_serialized_map

      def restored_map = Eval.me(string_containing_serialized_map)

      return restored_map
    
    }
    
    static def get_current_date_time() {
    
      Date date = new Date()
      
      def now = new Date()

      String timestamp = now.format("YYYY_MM_dd-HH:mm:ss")

      return timestamp
    }
    
    static def get_groovy_script_name() {
    
        def scriptName = getSimpleName()
    
        return scriptName
    }
    
    static def pretty_print_XML(some_xml){
    
       def langagesResult = new XmlSlurper().parseText(some_xml)

       def pretty_print_str = XmlUtil.serialize(langagesResult)
    
       return pretty_print_str
    }
    
    static def translate_string ( String input_string, Map translation_table ) {
    
       def translated_string = "tbd"
       if ( translation_table.containsKey( input_string ) ) {
         //println input_string + " => " + translation_table[input_string]
         translated_string = translation_table[input_string]
       }
       return translated_string
    }
    
    static def update_device_description ( String input_string, Map device_table ) {
    
       def device_description = "tbd"
       if ( device_table.containsKey( input_string ) ) {
         //println input_string + " => " + translation_table[input_string]
         device_description = device_table[input_string]
       }
       return device_description
    }
    
    static def create_xml_control_file(Map map, Map string_translation_maps, Map device_descriptions_map) {
        
        def parameters                       = map.parameters
        def devices_map                      = map.devices
        def master_parameter_translation_map = map.master_parameters
        def data_points_translation_map      = map.datapoints
        def paramset_translation_map         = map.paramsets
        
        def writer = new StringWriter()
        def xml = new MarkupBuilder(writer)
        
        def my_index = 0
        

        xml.AsksinPP_addon_generator_control() {
          addon_name( parameters["addon_name"] )
          addon_version( "tbd" )
          creator( parameters["creator"] )
          creator_prefix( parameters["creator_prefix"] )
          created_on( parameters["current_date_time"] )
          generator_version( parameters["VersionString"] )
          
          if ( devices_map.size() > 0 )
          {
            devices {
              devices_map.each { key, val ->
                def ind_str = my_index.toString()
                def device_id = key.toString() 
                device ( ("id"):"${key}", "index":ind_str, "device_name":val["device_name"], "device_model":val["device_model"], "device_small_case_name":val["device_small_case_name"],
                "rftypes_XML_version":val["device_model_version"], "VersionString":val["VersionString"]) {
                  device_description ( update_device_description( device_id, device_descriptions_map ) )
                }
                //device ("${key}")
                my_index = my_index + 1
              }
            }
          }

          if ( get_size_of_nested_map_depth_2(master_parameter_translation_map) > 0 )
          {
            //println master_parameter_translation_map
            paramset {
              master_parameter_translation_map.each { key0, val0 ->
                val0.each { key, val  ->             
                  def my_paramset_id = val["paramset_id"].toString()
                  def my_paramset_type = val["paramset_type"].toString()
                  def my_channel = val["channel_type"].toString()
                  def ind_str = my_index.toString()
                  parameter ( ("id"):"${key}", "index":ind_str, "paramset_type":my_paramset_type ,"paramset_id":my_paramset_id ) {
                    stringtable_identifier ( val["stringTable_name"] )
                    translation_de ( translate_string( val["stringTable_name"], string_translation_maps["de"] ) )
                    translation_en ( translate_string( val["stringTable_name"], string_translation_maps["en"] ) )
                    my_index = my_index + 1
                  }
                }
              }
            }
          }
          
          if ( get_size_of_nested_map_depth_2(data_points_translation_map) > 0 )
          {
            //println data_points_translation_map
            channels {
              data_points_translation_map.each { key0, val0 ->
                if ( val0.size() > 0 ) {
                  channel( ("type"):"${key0}") {
                    val0.each { key, val  ->
                      def my_paramset_id = val["paramset_id"].toString()
                      def my_paramset_type = val["paramset_type"].toString()
                      def my_channel = val["channel_type"].toString()
                      def ind_str = my_index.toString()
                      parameter ( ("id"):"${key}", "index":ind_str, "paramset_type":my_paramset_type ,"paramset_id":my_paramset_id, "channel":my_channel ) {
                        skip_channel_prefix ( "no" )
                        stringtable_identifier ( val["stringTable_name"] )
                        translation_de ( translate_string( val["stringTable_name"], string_translation_maps["de"] ) )
                        translation_en ( translate_string( val["stringTable_name"], string_translation_maps["en"] ) )
                        my_index = my_index + 1
                      }
                    }
                  }
                }
              }
            }
          }
          
          if ( get_size_of_nested_map_depth_2(paramset_translation_map) > 0 )
          {
            //println paramset_translation_map
            paramset_defs {
              paramset_translation_map.each { key0, val0 ->
                if ( val0.size() > 0 ) {
                  paramset( ("id"):"${key0}") {
                    val0.each { key, val  ->
                      def my_paramset_id = val["paramset_id"].toString()
                      def my_operations  = val["operations"].toString()
                      def my_channel = val["channel"].toString()
                      def my_key_prefix = "KEY"
                      def skip_paramset_prefix_flag = "yes"
                      if ( my_channel != "" ) {
                        my_key_prefix = my_channel
                        skip_paramset_prefix_flag = "no"
                      }
                      def ind_str = my_index.toString()
                      if ( val.containsKey("operations" ) && (my_operations != "" ) ) {
                        parameter ( ("id"):"${key}", "index":ind_str, "paramset_id":my_paramset_id, "operations":my_operations, "channel":my_channel ) {
                          stringtable_identifier ( val["stringTable_name"].toString() )
                          skip_paramset_prefix ( skip_paramset_prefix_flag )
                          paramset_prefix ( my_key_prefix )
                          translation_de ( translate_string( val["stringTable_name"], string_translation_maps["de"] ) )
                          translation_en ( translate_string( val["stringTable_name"], string_translation_maps["en"] ) )
                          my_index = my_index + 1
                        }
                      }
                      else {
                        parameter ( ("id"):"${key}", "index":ind_str, "paramset_id":my_paramset_id, "channel":my_channel ) {
                          stringtable_identifier ( val["stringTable_name"].toString() )
                          skip_paramset_prefix ( skip_paramset_prefix_flag )
                          paramset_prefix ( my_key_prefix )
                          translation_de ( translate_string( val["stringTable_name"], string_translation_maps["de"] ) )
                          translation_en ( translate_string( val["stringTable_name"], string_translation_maps["en"] ) )
                          my_index = my_index + 1
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        
        return writer.toString()
    }
    
    static def check_correctness_of_occu_path ( Map parameters ) {
      
      def directory_name = parameters.occu_path
      
      if ( directory_name[-1] == "/" ) {
        println "\n.. checking specified path of EQ3's occu repository .."
        println ".. removing '/' at end of occu path .."
        directory_name = directory_name[0..-2]
        println ".. updated occu path is '" + directory_name + "' .."
        parameters.occu_path = directory_name
      }
      
      def fh = new File(directory_name)
      
      if ( ! fh.isDirectory() ) {
        println "\nE R R O R : The specified path of EQ3's occu repository '" + directory_name + "' is not a directory!"
          println "Aborting .."
          System.exit(0)
      }
      
      def test_dir_name = directory_name + "/firmware/rftypes"
      def fh2 = new File(test_dir_name)
      
      if ( ! fh2.isDirectory() ) {
        println "\nE R R O R : The specified EQ3's occu repository '" + directory_name + "' does not contain a subdirectory '" + test_dir_name + "'!"
          println "Aborting .."
          System.exit(0)
      }
      
      test_dir_name = directory_name + "/WebUI/www/config/easymodes"
      fh2 = new File(test_dir_name)
      
      if ( ! fh2.isDirectory() ) {
        println "\nE R R O R : The specified EQ3's occu repository '" + directory_name + "' does not contain a subdirectory '" + test_dir_name + "'!"
          println "Aborting .."
          System.exit(0)
      }
    }
    
    static def get_list_of_xml_files(String directory_name) {
        
        def fh = new File(directory_name)
        
        // search for XML files with extension '*.xml'
        def p =  ~/.*\.xml/

        def xml_file_list = []
        def count = 0
        
        if ( fh.isDirectory() ) {
   
          fh.eachFileMatch(p) { xml_file ->
          
            def xml = new XmlSlurper().parse(xml_file)
            if ( xml.name() != "device" ) {
              println "W A R N I N G: The XML file '${xml_file}' is not a rftypes XML file! The root node is not 'device'!"
            }
            else {
              xml_file_list.add(xml_file)
              count++
            }
          }
        }
        else {
          println "E R R O R : The specified directory ${directory_name} is not a directory!\n"
          println "Aborting .."
          System.exit(0)
        }
        println "\n.. found ${count} XML files in directory '${directory_name}' ..\n"
        
        if ( count < 50 ) {
          println "\nW A R N I N G : The amount of XML files in the specified occu path '${directory_name}' is rather small: Only ${count} ! Please check the proper setting of the occu path!\n\n"
        }
        
        return xml_file_list
    }
    
    static def get_list_of_tcl_files( String directory_name, Map parameters ) {
    
        def tcl_file_list = []
        
        if ( ! parameters.no_easymodes ) {
    
          if ( directory_name[-1] == "/" ) {
            println "\n.. checking specified path for easymodes tcl files .."
            println ".. removing '/' at end of easymodes tcl path .."
            directory_name = directory_name[0..-2]
            println ".. updated easymodes tcl path is '" + directory_name + "' .."
          }
          
          def fh = new File(directory_name)
          
          if ( ! fh.isDirectory() ) {
            println "\nE R R O R : The specified path for easymodes tcl files '" + directory_name + "' is not a directory!"
              println "Aborting .."
              System.exit(0)
          }
          
          String required_match_regex = /^#!\/bin\/tclsh$/
          
          // search for TCL files with extension '*.'
          def p =  ~/.*\.tcl/

          
          def count = 0
          
          if ( fh.isDirectory() ) {

            fh.eachFileMatch(p) { tcl_file ->
            
              def first_line
              def fh_tcl = new File( tcl_file.toString() )
              
              fh_tcl.withReader { first_line = it.readLine() }  
              
              if ( ! ( first_line ==~ required_match_regex ) ) {
                println "W A R N I N G: The TCL file '${tcl_file}' might not a valid  TCL file! Please check! The first line is not '#!/bin/tclsh'!"
              }

              def file_name_without_extension = tcl_file.name.replaceFirst(~/\.[^\.]+$/, '')
              
              tcl_file_list.add(file_name_without_extension.toString())
              count++
              
            }
          }
          else {
            println "E R R O R : The specified directory ${directory_name} is not a directory!\n"
            println "Aborting .."
            System.exit(0)
          }
          println "\n.. found ${count} TCL files in directory '${directory_name}' ..\n"
        }
        
        return tcl_file_list
    }
    
    static def get_list_of_bck_files(String directory_name, Map parameters) {
    
        def fh = new File(directory_name)
        
        // search for XML files with extension '*.xml'
        def p =  ~/.*bck.*/

        def bck_file_list = []
        def count = 0
        
        if ( fh.isDirectory() ) {
   
          fh.eachFileMatch(p) { bck_file ->

              bck_file_list.add(bck_file)
              count++
          }
          println "\n.. found ${count} bck backup files in directory '${directory_name}' ..\n"
        }
        else {
          println "W A R N I N G : The specified directory ${directory_name} is not a directory!\n"
        }

        return bck_file_list
    }
    
    
    static def get_options(args, parameters) {
        def cli = new CliBuilder(usage: 'extract_rftypes_XML.groovy [options] [rftypes_XML_file_name]',
                                 header: 'options:',
                                 footer: "\nAsksinPP rftypes XML extractor ${VersionString} (c) FUEL4EP\n\n"
                                )
        
        // set the amount of columns the usage message will be wide
        cli.width = 100
        // Create the list of options.
        cli.with {
            h longOpt: 'help', 'Show usage information'
            occu longOpt: 'path_of_EQ3_occu_repository', args: 1, argName: 'occu_path', 'path of EQ-3\'s occu repository'
            easy longOpt: 'check_easymode_tcl_files', args: 1, argName: 'easy_path', 'path for checking for easymode tcl files default: ../src/addon/www/config/easymodes'
            ne longOpt: 'dont_extract_easymodes', args: 0, 'do not extract easymodes tcl files from easy path'
            o longOpt: 'output_XML_file', args: 1, argName: 'output_file_name', 'file name of generated XML control file'
            s longOpt: 'strict', args: 0, 'exact match of names without adding prefixes, e.g. channel_type'
            x longOpt: 'verbose', args: 0, 'verbose'
            eq3 longOpt: 'eQ-3_path', args: 1, argName: 'eq3_path', 'path to original EQ-3 files: webui.js, stringtable_de.txt, and translate.lang.stringtable.js'
            v longOpt: 'version', args: 0, 'version'
            a longOpt: 'addon_db', args: 1, argName: 'addon_db_file_name', 'file name of addon_db file which is containing all used addon names'
            r longOpt: 'restore_db', args: 0, 'restore addon_db'
            e longOpt: 'extract', args: 0, 'extract defined names in EQ-3\'s occu repository @ firmware/rftypes (requires option occu, too)'
            eq3_db longOpt: 'eQ-3_database', args: 1, argName: 'eq3_db', 'file name of eq3_db file which is containing all used EQ3 occu names'
            nb longOpt: 'no_addon_db_backup', args: 0, 'do not create a backup of the addon database if it already exits, but overwrite it'
            n longOpt: 'addon_name', args: 1, argName: 'addon_name', 'name of addon'
            t longOpt: 'translation_XML', args: 1, argName: 'translation_XML', 'file name of previous generated XML control file with translations'
            c longOpt: 'clean', args: 0, 'cleanup of all bck backup files, rename addon_db file'
            y longOpt: 'yes', args: 0, 'skip reconfirmations for yes/no inputs: yes is default'
            m longOpt: 'merge', args: 0, 'merge current addon_db with restored one'
        }
        
        def options = cli.parse(args)
        if (!options) {
            cli.usage()
            System.exit(0)
            return
        }
        // Show usage text when -h or --help option is used.
        if (options.h) {
            cli.usage()
            System.exit(0)
            return
        }
        
        
        
        
        if ( options.o ) {
          parameters.output_XML_file = options.o
        }
        else {
          parameters.output_XML_file = Addon_control_XML_file
        }
        if ( options.eq3 ) {
          parameters.eQ3_path = options.eq3
        }
        else {
          parameters.eQ3_path = "."
        }
        if ( options.a ) {
          parameters.addon_db_file_name = options.a
        }
        else {
          parameters.addon_db_file_name = Addon_translation_archive_file
        }
        if ( options.v ) {
          parameters.version   = true
        }
        else {
          parameters.version   = false
        }
        if ( options.s ) {
          parameters.strict    = true
        }
        else {
          parameters.strict    = false
        }
        if ( options.x ) {
          parameters.verbose   = true
        }
        else {
          parameters.verbose   = false
        }
        if ( options.occu ) {
          parameters.occu_path = options.occu
        }
        else {
          parameters.occu_path = "."
        }
        if ( options.easy ) {
          parameters.easymode_path = options.easy
        }
        else {
          parameters.easymode_path = Easymode_tcl_directory
        }
        if ( options.ne ) {
          parameters.no_easymodes    = true
        }
        else {
          parameters.no_easymodes    = false
        }
        if ( options.r ) {
          parameters.restore_db    = true
        }
        else {
          parameters.restore_db    = false
        }
        if ( options.e ) {
          parameters.extract_XMLs    = true
        }
        else {
          parameters.extract_XMLs    = false
        }
        if ( options.eq3_db ) {
          parameters.eq3_db = options.eq3_db
        }
        else {
          parameters.eq3_db = Occu_translation_archive_file
        }
        if ( options.nb ) {
          parameters.no_addon_db_backup    = true
        }
        else {
          parameters.no_addon_db_backup    = false
        }
        if ( options.n ) {
          parameters.addon_name = options.addon_name
        }
        else {
          parameters.addon_name = Addon_name
        }
        if ( options.t ) {
          parameters.translation_XML = options.translation_XML
        }
        else {
          parameters.translation_XML = Addon_control_XML_file_translation
        }
        if ( options.c ) {
          parameters.cleanup           = true
          parameters.restore_db        = false
          parameters.merge_addon_db    = false
        }
        else {
          parameters.cleanup   = false
        }
        if ( options.y ) {
          parameters.yes   = true
        }
        else {
          parameters.yes   = false
        }
        if ( options.m ) {
          parameters.restore_db        = true
          parameters.merge_addon_db    = true
        }
        else {
          parameters.merge_addon_db    = false
        }
    
        def extraArguments = options.arguments()
        
        if (extraArguments) {
          if ( extraArguments.size() == 1 ) {
            parameters.amount_extraArguments = 1
            parameters.extraArgument = extraArguments[0]
          }
          else {
            println "\nE R R O R :  Too many extra arguments, only one extra argument is allowed!\n"
            println "Aborting .."
            System.exit(0)
          }
        }
        else {
          parameters.amount_extraArguments = 0
        }
        
        return parameters
    } 
    
    
    static def get_size_of_nested_map_depth_2( Map nested_map ) {

        def count = 0
        nested_map.each { key, var ->
          count = count + var.size()
        }
        
        return count
        
    }
  
    
    static def print_nested_map_depth_2( Map nested_map, String str1, String str2 ) {

        nested_map.each{
          print "=====>"
          println ("${str1} : " + it.key )
          it.value.each {
              println ("${str2} ${it.key} : " + it.value)
          }
        }
    }
    
    static def check_if_paramset_is_an_easymode( String paramset_id, List easymodes_tcl_list, Map subset_map ) {
    
      Boolean is_easymode_flag = false
      String  keyset_comp_id
      
      // translate paramset_id by subset_map if it applies
      if ( subset_map.containsKey( paramset_id ) ) {
        keyset_comp_id = subset_map[paramset_id][0]
        //println "=>" + paramset_id
        //println "==>" + keyset_comp_id
      }
      else {
        keyset_comp_id = paramset_id
      }
      
      if ( easymodes_tcl_list.contains(keyset_comp_id) ) {
        is_easymode_flag = true
      }
      
      if ( is_easymode_flag ) {
        println ".. found easymode '" + paramset_id + "' .."
      }
      
      return is_easymode_flag
      
    }
    
    static def get_channel_from_subset_map( String paramset_id, Map subset_map ) {
    
      def channel_of_subset = ""
      
      if ( subset_map.containsKey( paramset_id ) ) {
        channel_of_subset = subset_map[paramset_id][1]
        //println "=>" + paramset_id
        //println "==> channel : " + channel_of_subset
      }
      
      return channel_of_subset
      
    }
  
    
    static def extract_rftypes_xml (Map db_map, Map eq3_db_map, my_xml_rftypes_XML_file_name, String string_prefix) {
    
        def devices_map                      = [:]
        def master_parameter_translation_map = [:]
        def data_points_translation_map      = [:]
        def paramset_translation_map         = [:]
        def easymodes_tcl_list               = []
        def parameters                       = [:]
        
        def webui_js_map                     = [:]
        def StringTable_de_map               = [:]
        def translate_lang_stringtable_map   = [:]
        
        def subset_map                       = [:]
        
       
        devices_map                          = db_map.devices
        master_parameter_translation_map     = db_map.master_parameters
        
        data_points_translation_map          = db_map.datapoints
        paramset_translation_map             = db_map.paramsets
        easymodes_tcl_list                   = db_map.easymodes
        parameters                           = db_map.parameters
       
        webui_js_map                         = eq3_db_map.webui_js
        StringTable_de_map                   = eq3_db_map.stringTable_de
        translate_lang_stringtable_map       = eq3_db_map.translate_lang_stringtable
        
        
        
        println "\n.. the specified rftypes XML file is '${my_xml_rftypes_XML_file_name}'"
            
        def device_name = ""
        def device_id   = ""
        def separator = ""
        
        def xml = new XmlSlurper().parse(my_xml_rftypes_XML_file_name)
        
        if ( xml.name() != "device" ) {
          println ""
          println "Fatal error: Not a rftypes device XML file '${xml.name()}'"
          println ""
          println "Aborting .."
          println ""
          System.exit(0)
        }
        
        if ( xml.supported_types.type.@name.size() > 1 ) {
        //  multiple devices
        xml.supported_types.type.@name.each {
          device_name = device_name + separator + it
          separator = "_"
        }
        xml.supported_types.type.@id.each {
          device_id   = device_id + separator + it
          separator = "_"
        }
        }
        else {
          //single device
          device_name = xml.supported_types.type.@name
          device_id   = xml.supported_types.type.@id
        }

        def device_model_version = xml.supported_types.type.parameter.@const_value[0].toString()
        def device_model         = xml.supported_types.type.parameter.@const_value[1].toString()
        //println xml.supported_types.type.parameter.@const_value.size()
        if (xml.supported_types.type.parameter.@const_value.size() == 1 ) {
          device_model         = device_model_version
          device_model_version = "not specified"
        }
        else {
          device_model     = xml.supported_types.type.parameter.@const_value[1]
        }
        println "\n\n======================================================================================================================\n\n"
        println "\n.. extracting rftypes XML file of ${device_id}.. \n\n"

        println ""
        println "device name          = ${device_name}"
        println "device id            = ${device_id}"
        println "XML version          = ${device_model_version}"
        println "device model         = ${device_model}"
        
        println ""
        
        parameters["VersionString"]          = VersionString.toString()
        parameters["device_name"]            = device_name.toString()
        parameters["device_id"]              = device_id.toString().toUpperCase()
        parameters["device_model_version"]   = device_model_version
        parameters["device_model"]           = device_model.toString()
        parameters["device_small_case_name"] = device_id.toString().replaceFirst("HB-","hb-").replaceFirst("hb-UNI-Sensor","hb-uni-sensor")
        
        // add entry to devices_map
        devices_map[parameters["device_id"]]         = ["device_name":parameters["device_name"], "device_small_case_name":parameters["device_small_case_name"], "device_model":parameters["device_model"], "device_model_version":parameters["device_model_version"], "VersionString":parameters["VersionString"]]
        
        def creator_prefix
        if ( string_prefix == "EQ3" ) {
          creator_prefix                     = "EQ3"
        }
        else
        {
          creator_prefix                     = device_model.toString().toUpperCase().substring(2,4)
        } 
        println "creator prefix       = ${creator_prefix}"
        println ""
        
        parameters["creator_prefix"]         = creator_prefix
        parameters["current_date_time"]      = get_current_date_time()
        parameters["creator"]                = System.getProperty("user.name").toString()

            
        xml.children().each {
          //println "1 '${it.name()}'"
          // parse node 'supported_types node'
          if (it.name() == "supported_types" )
          { //found node 'supported_types'
              //println "=> supported_types found"
          
          }
          if (it.name() == "paramset" )
          { if ( it.@type == "MASTER" ) {
              def paramset_type = it.@type.toString()
              def paramset_id   = it.@id.toString()
              if ( ! master_parameter_translation_map.containsKey(paramset_type) ) {
                master_parameter_translation_map[paramset_type]=[:]
              }
              it.children().each {
                def master_parameter_id
                if ( it.name() == "parameter" ) {
                  //println "=====> master_parameter '${it.@id}' found"
                  master_parameter_id = it.@id.toString()
                  def ui_flags = it.@ui_flags.toString()
                  if ( ui_flags != "invisible" ) {
                    master_parameter_translation_map[paramset_type][master_parameter_id]=["type":"device_parameters", "paramset_type":paramset_type, "paramset_id":paramset_id, "stringTable_name":"stringTable" + string_prefix + "_"+creator_prefix+"_"+master_parameter_id,"options":[]]
                    // check for options
                    it.children().each {
                      if ( ( it.name() == "logical" ) && ( it.@type == "option" ) ) {
                        it.children().each {
                          //println "=====>  option '${master_parameter_id}=${it.@id}'"
                          def my_option = master_parameter_id + "=" + it.@id.toString()
                          //println my_option
                          master_parameter_translation_map[paramset_type][my_option]=["type":"device_parameters", "paramset_type":paramset_type, "paramset_id":paramset_id, "stringTable_name":"stringTable" + string_prefix + "_"+creator_prefix+"_"+master_parameter_id+"_"+it.@id.toString(),"options":[my_option]]
                        }
                      }
                    }
                  }
                }
                if ( it.name() == "subset" ) {
                  println ".. adding to subset_map: " + it.@ref.toString() + " -> " + paramset_id + " @ device 'MASTER'"
                  subset_map[it.@ref.toString()] = [paramset_id, "MASTER"]
                }
              }
            }
          }
          if (it.name() == "channels" )
          { // found node 'channels'
            //println "=> channels found"
            it.children().each {
              if ( it.name() == "channel" ) {
                //println "=> channel  '${it.@type}' found"
                def channel_type = it.@type.toString()
                data_points_translation_map[channel_type]=[:]
                it.children().each {
                  if (it.name() == "paramset" )
                  { //println "=> paramset '${it.@type}' found"
                    def paramset_type = it.@type.toString()
                    def paramset_id   = it.@id.toString()

                    it.children().each {
                      def datapoint_id
                      if ( it.name() == "parameter" ) {
                        //println "=====> datapoint '${channel_type}|${it.@id}' found"
                        datapoint_id = it.@id.toString()
                        //println datapoint_id
                        data_points_translation_map[channel_type][datapoint_id]=["type":"channel", "paramset_type":paramset_type, "paramset_id":paramset_id, "channel_type":channel_type, "stringTable_name":"stringTable" + string_prefix + "_"+creator_prefix+"_"+datapoint_id,"options":[]]
                      }
                      // check for options
                      it.children().each {
                        if ( ( it.name() == "logical" ) && ( it.@type == "option" ) ) {
                          it.children().each {
                            //println "=====>  option '${datapoint_id}=${it.@id}'"
                            def my_option = datapoint_id + "=" + it.@id.toString()
                            //println my_option
                            data_points_translation_map[channel_type][my_option]=["type":"channel", "paramset_type":paramset_type, "paramset_id":paramset_id, "channel_type":channel_type, "stringTable_name":"stringTable" + string_prefix + "_"+creator_prefix+"_"+datapoint_id+"_"+it.@id.toString(),"options":[my_option]]
                          }
                        }
                      }
                      if ( it.name() == "subset" ) {
                        println ".. adding to subset_map: " + it.@ref.toString() + " -> " + paramset_id + " @ channel '" + channel_type + "'"
                        subset_map[it.@ref.toString()] = [paramset_id, channel_type]
                      }
                    }
                    
                  } 
                }
              }
            }
          }
          if (it.name() == "paramset_defs" )
          { //println "=> paramset_defs found"
              it.children().each {
              if (it.name() == "paramset" )
              { //println "=> paramset '${it.@id}' found"
                def paramset_type = it.@type.toString()
                def paramset_id     = it.@id.toString()
                //println paramset_id
                //println subset_map
                //println easymodes_tcl_list
                if ( check_if_paramset_is_an_easymode( paramset_id, easymodes_tcl_list, subset_map ) ) {
                  //println check_if_paramset_is_an_easymode( paramset_id, easymodes_tcl_list, subset_map )
                }
                else {
                  //println check_if_paramset_is_an_easymode( paramset_id, easymodes_tcl_list, subset_map )
                  //println "=>>>> channel: " + get_channel_from_subset_map( paramset_id, subset_map )
                  paramset_translation_map[paramset_id]=[:]
                  it.children().each {
                    if (it.name() == "parameter" )
                    { 
                      //println "===== KEY of '${paramset_id}': 'KEY|${it.@id}' found"
                      def key_id = it.@id.toString()
                      def my_operations = it.@operations.toString()
                      def my_control = it.@control.toString()
                      //println "KEY: " + key_id
                      paramset_translation_map[paramset_id][key_id]=["type":"paramset_defs", "paramset_type":paramset_type, "paramset_id": paramset_id, "stringTable_name":"stringTable" + string_prefix + "_"+creator_prefix+"_"+key_id,"options":[], "operations":my_operations, "control":my_control, "channel":get_channel_from_subset_map( paramset_id, subset_map )]
                      //println paramset_translation_map[paramset_id][key_id]
                      it.children().each {
                        if ( ( it.name() == "logical" ) && ( it.@type == "option" ) ) {
                          it.children().each {
                              //println "=====>  option 'KEY|${key_id}': '${it.@id}'"
                              def my_key_option = key_id + "=" + it.@id.toString()
                              paramset_translation_map[paramset_id][my_key_option]=["type":"paramset_defs", "paramset_type":paramset_type, "paramset_id": paramset_id, "stringTable_name":"stringTable" + string_prefix + "_"+creator_prefix+"_"+key_id+"_"+it.@id.toString(),"options":[my_key_option], "operations":my_operations, "control":my_control, "channel":get_channel_from_subset_map( paramset_id, subset_map )]
                          }
                        }
                      }
                    }
                  }
                }
              }
            } 
          }
        } 
        
        //println ""
        //println master_parameter_translation_map
        //println data_points_translation_map
        //println paramset_translation_map
        //println ""
            
        if ( parameters.verbose ) {
        
            println "\n\nExtracted subset map:\n\n"
            
            println subset_map
            
            println ""
            
            println "\n\nExtracted master parameter candidates for translation are:\n\n"
            
            print_nested_map_depth_2(master_parameter_translation_map,"channel", "parameter")
                
            println "\n\nExtracted channel parameter candidates for translation are:\n\n"
            
            print_nested_map_depth_2(data_points_translation_map,"channel", "parameter")
                
            println "\n\nExtracted paramset parameter candidates for translation are:\n\n"
            
            print_nested_map_depth_2(paramset_translation_map,"paramset", "parameter")
            
        }
        
        println "\n.. removing duplicated entries for ${device_id} .. \n\n"
        
        master_parameter_translation_map = remove_duplicates_from_translation_map(master_parameter_translation_map, webui_js_map, StringTable_de_map, translate_lang_stringtable_map, parameters)
        
        data_points_translation_map      = remove_duplicates_from_translation_map(data_points_translation_map, webui_js_map, StringTable_de_map, translate_lang_stringtable_map, parameters)
        
        paramset_translation_map             = remove_duplicates_from_translation_map(paramset_translation_map, webui_js_map, StringTable_de_map, translate_lang_stringtable_map, parameters)
        
        if ( parameters.verbose ) {
        
          println "\n\nRemaining master parameter candidates for translation are:\n\n"
        
          print_nested_map_depth_2(master_parameter_translation_map,"channel", "parameter")
        
          println "\n\nRemaining data point candidates for translation are:\n\n"
          
          print_nested_map_depth_2(data_points_translation_map,"channel", "parameter")
        
          println "\n\nRemaining paramset candidates for translation are:\n\n"
        
          print_nested_map_depth_2(paramset_translation_map,"paramset", "parameter")
          
          println ".. the master_parameter_translation_map has " + get_size_of_nested_map_depth_2(master_parameter_translation_map) + " entries"
          println ".. the data_points_translation_map has " + get_size_of_nested_map_depth_2(data_points_translation_map) + " entries"
          println ".. the paramset_translation_map has " + get_size_of_nested_map_depth_2(paramset_translation_map) + " entries"
        }
        
        db_map.devices                       = devices_map
        db_map.master_parameters             = master_parameter_translation_map
        db_map.datapoints                    = data_points_translation_map
        db_map.paramsets                     = paramset_translation_map
        db_map.easymodes                     = easymodes_tcl_list
        db_map.parameters                    = parameters
        
        return db_map
    }
    
    static def input_yes( Map parameters ) {
        
        def correct_input = false
        def answer = false
        def yes_regex = /[Yy](es)?/
        def no_regex = /[Nn](o)?/
        
        if ( ! parameters.yes ) {

          while( ! correct_input ) {
            println "\nPlease enter [Yy]es or [Nn]o\n"
            def input = System.in.newReader().readLine() as String
            if ( input ==~ yes_regex ) {
              answer = true
              break
            }
            if ( input ==~ no_regex ) {
              correct_input = true
              break
            }
          }
        }
        else {
          println ".. default answer 'yes' has been set as CLI parameter .."
          answer = true
        }
        
        return answer
    }
    
    static def cleanup_files( parameters ) {
        // deleting bck backup files in current directory  '.'
        println "\n.. cleaning up bck backup files\n\n"
        def bck_file_list = get_list_of_bck_files( ".", parameters )
        if ( bck_file_list.size() > 0 ) {
          println "\n.. do you want these files to be deleted?\n"
          bck_file_list.each {
            println "  " + it
          }
          if ( input_yes( parameters ) ) {
            println ".. deleting files now ..\n"
            bck_file_list.each {
              println ".. deleting file  '" + it + "'"
              def file = new File( it.toString() )
              file.delete()
            }
          }
        }
        // deleting bck backup files in subdirectory  './libaries'
        bck_file_list = get_list_of_bck_files( "./libraries", parameters )
        if ( bck_file_list.size() > 0 ) {
          println "\n.. do you want these files to be deleted?\n"
          bck_file_list.each {
            println "  " + it
          }
          if ( input_yes( parameters ) ) {
            println ".. deleting files now ..\n"
            bck_file_list.each {
              println ".. deleting file  '" + it + "'"
              def file = new File( it.toString() )
              file.delete()
            }
          }
        }
        // delete addon_db data base if it exists
        if ( check_existance_of_file ( parameters.addon_db_file_name ) ) {
          println "\n.. do you want the addon database file '${parameters.addon_db_file_name}' be deleted?\n"
          if ( input_yes( parameters ) ) {
            println ".. deleting file  '" + parameters.addon_db_file_name + "'"
            def file = new File( parameters.addon_db_file_name )
            file.delete()
          }
        }
        // delete eq3_db data base if it exists
        if ( check_existance_of_file ( parameters.eq3_db ) ) {
          println "\n.. do you want the EQ3 database file '${parameters.eq3_db_file_name}' be deleted?\n"
          if ( input_yes( parameters ) ) {
            println ".. deleting file  '" + parameters.eq3_db + "'"
            def file = new File( parameters.eq3_db )
            file.delete()
          }
        }
    }
    
    static def merge(Map lhs, Map rhs) {
        return rhs.inject(lhs.clone()) { map, entry ->
        //println "Merge Key => " + entry.key
        if ( entry.key == "parameters" ) {
          return map
        }
        //println " Key : " + key.getClass() 
        /*println " Map            : " + map.getClass() 
        println " entry.key      : " + entry.key.getClass() 
        println " map[entry.key] : " + map[entry.key].getClass() 
        println " entry.value: " + entry.value.getClass()*/
          if (map[entry.key] instanceof Map && entry.value instanceof Map) {
              /*println entry.value instanceof Map
              println "Map : " + ".. found Map " + entry.key + " =>          Map : " + map[entry.key] + "            Value : " + entry.value*/
              map[entry.key] = merge(map[entry.key], entry.value)
          } else if (map[entry.key] instanceof Collection && entry.value instanceof Collection) {
              /*println "Collection : " + entry.value instanceof Collection
              println ".. found Collection " + entry.key + " => " + entry.value*/
              map[entry.key] += entry.value
          } else {
              //println "..found Value " + entry.key + " => " + entry.value
              map[entry.key] = entry.value
          }
          return map
        }
    }
    
    static def merge_maps( Map addon_db_map, Map restore_db_map, Map parameters ) {
    
      Map.metaClass.merge << { Map rhs -> merge(delegate, rhs) } 
      
      def merged_db_map = [:]
      
      /*println restore_db_map
      
      println addon_db_map*/
      
      if ( parameters.verbose ) {
        println "\n\n.. before merging the Addon master_parameter_translation_map has " + get_size_of_nested_map_depth_2(addon_db_map.master_parameters) + " entries"
        println ".. before merging the Addon data_points_translation_map has " + get_size_of_nested_map_depth_2(addon_db_map.datapoints) + " entries"
        println ".. before merging the Addon paramset_translation_map has " + get_size_of_nested_map_depth_2(addon_db_map.paramsets) + " entries"
        
        println "\n\n.. before merging the restored Addon master_parameter_translation_map has " + get_size_of_nested_map_depth_2(restore_db_map.master_parameters) + " entries"
        println ".. before merging the restored Addon data_points_translation_map has " + get_size_of_nested_map_depth_2(restore_db_map.datapoints) + " entries"
        println ".. before merging the restored Addon paramset_translation_map has " + get_size_of_nested_map_depth_2(restore_db_map.paramsets) + " entries\n\n"
      }
      

      merged_db_map = addon_db_map.merge(restore_db_map)
      
      if ( parameters.verbose ) {
        println "\n.. merged_db_map ..\n\n"
        println merged_db_map
      }
      
      println "\n\n.. the merged Addon devices_map has " + merged_db_map.devices.size() + " entries"
      println ".. the merged Addon master_parameter_translation_map has " + get_size_of_nested_map_depth_2(merged_db_map.master_parameters) + " entries"
      println ".. the merged Addon data_points_translation_map has " + get_size_of_nested_map_depth_2(merged_db_map.datapoints) + " entries"
      println ".. the merged Addon paramset_translation_map has " + get_size_of_nested_map_depth_2(merged_db_map.paramsets) + " entries"
    
      return merged_db_map
    }

    static void main(String... args) {
    
        //Map for program parameters
        def parameters = [:]
        
        parameters = get_options(args, parameters)
        
        println "\nAsksinPP rftypes XML extractor ${VersionString}\n\n"
        println "\nCurrent data and time : " + get_current_date_time() + "\n\n"
        
        if ( parameters.version ) {
          System.exit(0)
        }
        
        if ( parameters.cleanup ) {
          cleanup_files ( parameters )
        }
        
        def my_xml_rftypes_XML_file_name
     
        if ( parameters.amount_extraArguments == 0 ) {
          my_xml_rftypes_XML_file_name = get_control_XML_file_name()
        }
        
        if ( parameters.amount_extraArguments == 1 ) {
          my_xml_rftypes_XML_file_name = check_control_XML_file_name(parameters.extraArgument)
        }
        
        println "\n.. working on XML file '${my_xml_rftypes_XML_file_name}’\n\n"
        
        // parse the EQ3 files webui.js, stringtable_de.txt, translate.lang.stringtable.js and store the extracted names in the maps webui_js_map, StringTable_de_map, translate_lang_stringtable_map
        def my_file_name
        my_file_name = check_file(parameters.eQ3_path + "/" + "webui.js")
        def webui_js_map                   =  parse_webui_js(my_file_name)
        my_file_name = check_file(parameters.eQ3_path + "/" + "stringtable_de.txt")
        def StringTable_de_map             =  parseStringTable(my_file_name)
        my_file_name = check_file(parameters.eQ3_path + "/" + "translate.lang.stringtable.js")
        def translate_lang_stringtable_map = parseJson(my_file_name)

        // define occu maps
        def occu_db_map                      = [:]
        // define eq3_map for webui.js, stringtable_de.txt, translate.lang.stringtable.js
        def eq3_db_map                       = [:]
        
        // extract defined names in EQ3's OCCU repository at ${parameters.occu_path}/firmware/rftypes/*.xml
        if ( parameters.extract_XMLs ) {
        
          check_correctness_of_occu_path( parameters )

          def devices_map                      = [:]
          def master_parameter_translation_map = [:]
          def data_points_translation_map      = [:]
          def paramset_translation_map         = [:]
          def restored_database_maps           = [:]
          def easymodes_tcl_list               = []
          
          //store maps into master occu_db_map
          occu_db_map.devices                       = devices_map
          occu_db_map.master_parameters             = master_parameter_translation_map
          occu_db_map.datapoints                    = data_points_translation_map
          occu_db_map.paramsets                     = paramset_translation_map
          occu_db_map.easymodes                     = easymodes_tcl_list
          occu_db_map.parameters                    = parameters
          
          eq3_db_map.webui_js                       = webui_js_map
          eq3_db_map.stringTable_de                 = StringTable_de_map
          eq3_db_map.translate_lang_stringtable     = restored_database_maps
          
        
          // set prefix for EQ3 devices
          def string_prefix             = "EQ3"
          def list_of_xml_files         = []
          def list_of_easymode_tc_files = []
          def easymode_tcl_directory    = parameters.occu_path + "/WebUI/www/config/easymodes"
          def rftypes_xml_directory     = parameters.occu_path + "/firmware/rftypes"
          
          
          println "\n.. extracting now easymode tcl files located in EQ-3\'s occu repository @ '${easymode_tcl_directory}/*.tcl' .. \n"
          easymodes_tcl_list = get_list_of_tcl_files( easymode_tcl_directory, parameters )
          occu_db_map.easymodes                     = easymodes_tcl_list
          
          if ( parameters.verbose ) {
            println ".. extracted easymode tcl file names are:\n" 
            println easymodes_tcl_list
            println ""
          }
          
          println "\n\n======================================================================================================================"
          println "======================================================================================================================\n\n"
          println "\n.. extracting now defined names in the specified EQ-3\'s occu repository @ '${parameters.occu_path}/firmware/rftypes/*.xml' .. \n"
          list_of_xml_files = get_list_of_xml_files( rftypes_xml_directory )
          
          list_of_xml_files.each {
            println "\n\n.. working on '${it}' ..\n"
            occu_db_map = extract_rftypes_xml(occu_db_map, eq3_db_map, it, string_prefix )
            
          }
          
          println "\n\n.. the EQ3 Occu devices_map has " + occu_db_map.devices.size() + " entries"
          println ".. the EQ3 Occu master_parameter_translation_map has " + get_size_of_nested_map_depth_2(occu_db_map.master_parameters) + " entries"
          println ".. the EQ3 Occu data_points_translation_map has " + get_size_of_nested_map_depth_2(occu_db_map.datapoints) + " entries"
          println ".. the EQ3 Occu paramset_translation_map has " + get_size_of_nested_map_depth_2(occu_db_map.paramsets) + " entries"
          println ".. the EQ3 Occu easymodes tcl list has " + occu_db_map.easymodes.size() + " entries"
          
          //save database
           save_maps_to_file(occu_db_map, parameters, parameters.eq3_db)
          
          println "\n\n.. finished extracting defined names in the specified EQ-3\'s occu repository @ '${parameters.occu_path}/firmware/rftypes/*.xml' .. \n"
          println "======================================================================================================================"
          println "======================================================================================================================\n\n"
        }
        
        
        // define addon maps
        def restore_db_map                   = [:]
        def addon_db_map                     = [:]
            eq3_db_map                       = [:]
        def devices_map                      = [:]
        def master_parameter_translation_map = [:]
        def data_points_translation_map      = [:]
        def paramset_translation_map         = [:]
        def easymodes_tcl_list               = []
        def string_translation_maps          = [:]
        def device_descriptions_map          = [:]
        def occu_easymodes_tcl_list          = []
        
        
        // restore addon_db_map from previous run if specified as command line parameter
        if ( parameters.restore_db ) {
          // restore database
          println parameters.addon_db_file_name
          println "\nRestoring database from file '${parameters.addon_db_file_name}' .. \n"
          restore_db_map = read_maps_from_file(parameters.addon_db_file_name)
          
           if ( parameters.verbose ) {
            println "\nRestored database '${parameters.addon_db_file_name}'\n"
            println "\n.. restored map of devices:\n"
            println restore_db_map.devices
            println "\n.. restored map of master parameters:\n"
            println restore_db_map.master_parameters
            println "\n.. restored map of datapoints:\n"
            println restore_db_map.datapoints
            println "\n.. restored map of paramsets:\n"
            println restore_db_map.paramsets
            println "\n.. restored list of easymodes:\n"
            println restore_db_map.easymodes
            println ""
          }
        }
        
        //store maps into master addon_db_map
        addon_db_map.devices                       = devices_map
        addon_db_map.master_parameters             = master_parameter_translation_map
        addon_db_map.datapoints                    = data_points_translation_map
        addon_db_map.paramsets                     = paramset_translation_map
        addon_db_map.easymodes                     = easymodes_tcl_list
        addon_db_map.parameters                    = parameters
        eq3_db_map.webui_js                        = webui_js_map
        eq3_db_map.stringTable_de                  = StringTable_de_map
        eq3_db_map.translate_lang_stringtable      = translate_lang_stringtable_map
        

        def easymode_tcl_directory                 = parameters.easymode_path 
        
        println "\n.. extracting now easymode tcl files located in specified easymodes tcl directory @ '${parameters.easymode_path}/*.tcl' .. \n"
        easymodes_tcl_list = get_list_of_tcl_files( easymode_tcl_directory, parameters )
        
        def occu_easymode_tcl_directory    = parameters.occu_path + "/WebUI/www/config/easymodes"
        println "\n.. extracting now easymode tcl files located in EQ-3\'s occu repository @ '${occu_easymode_tcl_directory}/*.tcl' .. \n"
        occu_easymodes_tcl_list = get_list_of_tcl_files( occu_easymode_tcl_directory, parameters )
        
        //merge the two easymode tcl lists
        def merged_easymodes_tcl_list = easymodes_tcl_list + occu_easymodes_tcl_list
        //unify the merged easymode tcl list
        def unique_merged_easymodes_tcl_list = merged_easymodes_tcl_list.unique()

          
        addon_db_map.easymodes                     = unique_merged_easymodes_tcl_list
          
        if ( parameters.verbose ) {
          println ".. extracted easymode tcl file names are:\n" 
          println easymodes_tcl_list
          println ""
        }
        
        
        // extract the specified HB rftypes XML file
        // set prefix for homebrew devices
        def string_prefix = "HB"
        addon_db_map = extract_rftypes_xml(addon_db_map, eq3_db_map, my_xml_rftypes_XML_file_name, string_prefix )
        
        
        if ( parameters.extract_XMLs ) {
          // remove duplicated entries of addon_db_map and occu_db_map submaps
          println "\n\n.. removing duplicated entries of  addon_db_map and occu_db_map ..\n"
          addon_db_map = remove_duplicates_from_addon_map(addon_db_map, occu_db_map, parameters)
        }
        
        if ( parameters.restore_db ) {
          // remove duplicated entries of addon_db_map and restore_db_map submaps, i.e. remove already existing entries from previous runs
          if ( parameters.verbose ) {
            println "\nAddon_map\n\n"
            println addon_db_map
            println "\nRestored_map\n\n"
            println restore_db_map
          }
        
          println "\n\n.. removing duplicated entries of  addon_db_map and restore_db_map ..\n"
          addon_db_map = remove_duplicates_from_addon_map(addon_db_map, restore_db_map, parameters)
          
        }
        
        println "\n\n.. the Addon device_map has " + addon_db_map.devices.size() + " entries"
        println ".. the Addon master_parameter_translation_map has " + get_size_of_nested_map_depth_2(addon_db_map.master_parameters) + " entries"
        println ".. the Addon data_points_translation_map has " + get_size_of_nested_map_depth_2(addon_db_map.datapoints) + " entries"
        println ".. the Addon paramset_translation_map has " + get_size_of_nested_map_depth_2(addon_db_map.paramsets) + " entries"
        println ".. the Addon easymodes tcl list has " + addon_db_map.easymodes.size() + " entries"
        
        // extract German and English translation maps from a previous XML control file for addon generator (CLI paramater 't')
        string_translation_maps = extract_string_translations_from_previous_xml_control_file( parameters.translation_XML.toString() )
        
        // extract device descriptions map from a previous XML control file for addon generator (CLI paramater 't')
        device_descriptions_map = extract_device_descriptions_from_previous_xml_control_file( parameters.translation_XML.toString() )
        
        // merge addon_db_map and restore_db_map in order to build-up the addon data base for a set of devices
        if ( parameters.merge_addon_db ) {
          println "\n\n.. merging addon_db_map and restore_db_map ..\n"
          addon_db_map = merge_maps( addon_db_map, restore_db_map, parameters )
        }
        
        println ".. saving addon data base to '${parameters.addon_db_file_name}' .. "
        //save database
        save_maps_to_file(addon_db_map, parameters, parameters.addon_db_file_name)
        
        
        def XML_control_file = parameters.output_XML_file

        println "\n\n.. creating XML control file for addon generator '" + XML_control_file + "'\n\n"
        
        def XML_text = create_xml_control_file(addon_db_map, string_translation_maps, device_descriptions_map)
        
        write_control_xml_file( XML_text, XML_control_file)
        
        println "\n.. done\n"
      }
    }
