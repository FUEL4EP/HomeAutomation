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


class extract_rftypes_XML {

    static VersionString                  = "0.10"
    static Map_Storage_Identifier         = "extract_rftypes_XML map storage file, please don't touch!\n\n"
    static Addon_translation_archive_file = "extract_rftypes_XML_translation_archive.db"
    
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
        
        String required_match_regex = /^jQuery\.extend\(true\,langJSON\,\ \{\n\ \ \"de\"\ \:\ [\s\S]*\}\);$/


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

    
    static def check_control_XML_file_name(String rftypes_XML_file_name) {
       
        println "\nThe specified rftypes XML file name is :  '${rftypes_XML_file_name}'"
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
            println("The specified rftypes XML file '${rftypes_XML_file_name}' is not a well-formed XML file")
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
    
    static def check_if_key_already_exists( key, Map translation_map, Map webui_js_map, Map StringTable_de_map, Map translate_lang_stringtable_map ) {
    
        def key_already_exists = false

        if ( webui_js_map.containsKey(key) ) {
            println "=> '${key}' already defined in 'webui.js'"
            key_already_exists = true
        }
        else {
          if ( StringTable_de_map.containsKey(key) )  {
            println "=> '${key}' already defined in 'stringtable_de.txt'"
            key_already_exists = true
          }
          else {
            if ( translation_map.containsKey(key) ) {
              def my_stringtable_key = translation_map[key]."stringTable_name"
              if ( translate_lang_stringtable_map.containsKey(my_stringtable_key) ) {
                println "=> '${key}' already defined in 'translate.lang.stringtable.js'"
                key_already_exists = true
              }
            }
          }
        }
        
        //println key_already_exists
          
        return key_already_exists
    
    }
    
    
    static def remove_duplicates_from_translation_map(Map translation_map, Map webui_js_map, Map StringTable_de_map, Map translate_lang_stringtable_map) {
    
        def keys_to_be_deleted = []
        
    
        translation_map.each{ key, var ->
        
          def key_str = ""
          
          /*println "key => '${key}'"
          println "var = " + var["type"]
          println "var = " + var["channel_type"]
          println "'${key}'"*/
          
          key_str = key
          
          def key_already_exists = false
          
          key_already_exists |= check_if_key_already_exists ( key, translation_map, webui_js_map, StringTable_de_map, translate_lang_stringtable_map)
          if ( var["type"] == "channel" ) {
            key_str = "${var["channel_type"]}|" + key
            //println "channel => ${key_str}"
            key_already_exists |= check_if_key_already_exists ( key_str, translation_map, webui_js_map, StringTable_de_map, translate_lang_stringtable_map)
          }
          if ( var["type"] == "param_defs" ) {
            key_str = "KEY|" + key
            //println "channel => ${key_str}"
            key_already_exists |= check_if_key_already_exists ( key_str, translation_map, webui_js_map, StringTable_de_map, translate_lang_stringtable_map)
          }
          if ( key_already_exists ) {
            keys_to_be_deleted.add(key)
          }
        }
        // remove duplicated entries in data_points_translation_map
        keys_to_be_deleted.each {
          translation_map.remove(it)
        }
        
        return translation_map
    }
    
    static def save_map_to_file (Map map_to_be_saved, String file_name) {
    
      File file = new File(file_name)
      
      if ( file.exists() ){
        println "\n\nW A R N I N G: The file '${file_name}' is already existing!"
        println "File '${file_name}' will be renamed to '" + file_name + "." + get_current_date_time()  + ".bck'"
        file.renameTo(file_name + ".bck_" + get_current_date_time())
        file = new File(file_name)
      }
      
      def string_containing_serialized_map = map_to_be_saved.inspect()
      
      file.write(Map_Storage_Identifier)
      file.append("created by '" + get_groovy_script_name() + "' " + VersionString + "\n")
      file.append("creation date : " + get_current_date_time() + "\n")
      file.append("creator : " + System.getProperty("user.name") + "\n\n")
        
      file.append(string_containing_serialized_map) 
    
    }
    
    static def read_map_from_file (String file_name) {
    
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
      string_containing_serialized_map = string_containing_serialized_map - ~/^.*\n\ncreated by.*\ncreation date.*\n.*\n\n/
      
      def restored_map = Eval.me(string_containing_serialized_map)
      
      //return restored_map
    
    }
    
    static def get_current_date_time() {
    
      Date date = new Date()
      
      def now = new Date()

      String timestamp = now.format("YYYYMMdd-HH:mm:ss")

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
    
   static def makeXmlBody(map, lastParent, counter) {
        // credits to Asinus Rex, see https://stackoverflow.com/questions/48709822/dynamically-generate-xml-with-attributes-and-child-nodes-from-map-in-groovy
        def writer = new StringWriter()
        def xmlBuilder = new MarkupBuilder(writer)
        if (counter == 0) {
            xmlBuilder.mkp.xmlDeclaration(version: "1.0", encoding: "utf-8")
        }
        counter++

        map.each {
            if (it.value.getClass() == LinkedHashMap) {
                xmlBuilder.createNode(it.key, it.value.attributes, makeXmlBody(it.value.elements, lastParent, counter))
                xmlBuilder.nodeCompleted(lastParent, it.key)
                lastParent = it.key
            } else {

                xmlBuilder.createNode(it.key, it.value)
                xmlBuilder.nodeCompleted(lastParent, it.key)
            }
    }
    
    //unescape 
    def escaped_str = writer.toString().replaceAll(/&lt;/, '<')
         .replaceAll(/&gt;/, '>')
         .replaceAll(/&quot;/, '"')
         .replaceAll(/&apos;/, "'")
         .replaceAll(/&amp;/, '&')

        
    return escaped_str
}

/*

       Map map = [
            root: [
                    elements  : [
                            node1: [
                                    elements  : [
                                            key1Node1: "elementValue1Node1",
                                            key2Node1: "elementValue2Node2",
                                    ],
                                    attributes: [
                                            attribute1Node1: "attributeValue1node1",
                                            attribute2Node1: "attributeValue2node1",
                                            attribute3Node1: "attributeValue3node1"
                                    ]
                            ],
                            node2: [
                                    elements  : [
                                            key1Node2: "elementValue1Node2",
                                            key2Node2: "elementValue2Node2",
                                    ],
                                    attributes: [
                                            attribute1Node2: "attributeValue1node2",
                                            attribute2Node2: "attributeValue2node2",
                                            attribute3Node2: "attributeValue3node2"
                                    ]
                            ]
                    ],
                    attributes: [
                            rootAttribute: "iniciarSesion"
                    ]
            ]

    ]
*/
    
    static def create_map_for_xml_generation (VersionString, device_name, device_id){
    
        def map_for_xml_generation = [:]
        
        //map_for_xml_generation["AsksinPP_addon_control_file"] = ["elements" : ["creator":"FUEL4EP"],"attributes" : [rootAttribute: "my attribute"]]
        
        map_for_xml_generation = [
            AsksinPP_addon_generator_control: [
                    elements  : [
                            creator                : System.getProperty("user.name"),
                            created_on             : get_current_date_time(),
                            device_name            : device_name,
                            device_id              : device_id,
                            device_small_case_name : device_id.toString().replaceFirst("HB-","hb-").replaceFirst("hb-UNI-SENSOR","hb-uni-sensor"),
                            device_description     : "Please subsitute appropriately",
                            version                : "1.0", 
                            translated_datapoints: [
                                    elements  : [
                                            key1Node1  : "elementValue1Node1",
                                            key2Node1  : "elementValue2Node2",
                                    ],
                                    attributes: [
                                            type: "channel"
                                    ]
                            ],
                            node2: [
                                    elements  : [
                                            key1Node2: "elementValue1Node2",
                                            key2Node2: "elementValue2Node2",
                                    ],
                                    attributes: [
                                            attribute1Node2: "attributeValue1node2",
                                            attribute2Node2: "attributeValue2node2",
                                            attribute3Node2: "attributeValue3node2"
                                    ]
                            ]
                    ],
                    attributes: [
                            Version: VersionString
                    ]
            ]

    ]
        
        println "\nXML generation ...\n"
    
        def tt = makeXmlBody(map_for_xml_generation, null, 0)
        
        println pretty_print_XML(tt)
    }

    static void main(String... args) {
        
        println "\nAsksinPP rftypes XML extractor ${VersionString}\n\n"
        
        def my_xml_rftypes_XML_file_name
     
        if ( args.size() == 0 ) {
          my_xml_rftypes_XML_file_name = get_control_XML_file_name()
        }
        
        if ( args.size() == 1 ) {
          my_xml_rftypes_XML_file_name = check_control_XML_file_name(args[0])
        }
        
        if ( args.size() > 1 ) {
          println "Too many command line parameters !"
          println "\nUsage: groovy AsksinPP_addon_generator.groovy [<xml_rftypes_XML_file_name>]\n"
          println "Aborting .."
          System.exit(0)
        }
        
        println "rftypes XML file : ${my_xml_rftypes_XML_file_name}"
        
        def xml = new XmlSlurper().parse(my_xml_rftypes_XML_file_name)

        def device_name = xml.supported_types.type.@name
        def device_id   = xml.supported_types.type.@id

        def device_model_version = xml.supported_types.type.parameter.@const_value[0]
        def device_model         = xml.supported_types.type.parameter.@const_value[1]
        //println xml.supported_types.type.parameter.@const_value.size()
        if (xml.supported_types.type.parameter.@const_value.size() == 1 ) {
          device_model         = device_model_version
          device_model_version = "not specified"
        }
        else {
          device_model     = xml.supported_types.type.parameter.@const_value[1]
        }

        if ( xml.name() != "device" ) {
          println ""
          println "Fatal error: Not a rftypes device XML file '${xml.name()}'"
          println ""
          println "Aborting .."
          println ""
          System.exit(0)
        }
        println ""
        println "device name          = ${device_name}"
        println "device id            = ${device_id}"
        println "XML version          = ${device_model_version}"
        println "device model         = ${device_model}"
        println ""
        
        def creator_prefix            = device_model.toString().toUpperCase().substring(2,4)
        println "creator prefix       = ${creator_prefix}"
        println ""
        
        //initialize the translation maps
        def master_parameter_translation_map = [:]
        def data_points_translation_map      = [:]
        def keys_translation_map             = [:]
        
        xml.children().each {
          //println "1 '${it.name()}'"
          // parse node 'supported_types node'
          if (it.name() == "supported_types" )
          { //found node 'supported_types'
            //println "=> supported_types found"
           
          }
          if (it.name() == "paramset" )
          { if ( it.@type == "MASTER" ) {
              it.children().each {
                def master_parameter_id
                if ( it.name() == "parameter" ) {
                  //println "=====> master_parameter '${it.@id}' found"
                  master_parameter_id = it.@id.toString()
                  def ui_flags = it.@ui_flags.toString()
                  if ( ui_flags != "invisible" ) {
                    master_parameter_translation_map[master_parameter_id]=["type":"MASTER","stringTable_name":"stringTableHB"+"_"+creator_prefix+"_"+master_parameter_id,"options":[]]
                    // check for options
                    it.children().each {
                      if ( ( it.name() == "logical" ) && ( it.@type == "option" ) ) {
                        it.children().each {
                          //println "=====>  option '${master_parameter_id}=${it.@id}'"
                          def my_option = master_parameter_id + "=" + it.@id.toString()
                          //println my_option
                          master_parameter_translation_map[master_parameter_id]."options".add(my_option)
                        }
                      }
                    }
                  }
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
                it.children().each {
                  if (it.name() == "paramset" )
                  { //println "=> paramset '${it.@type}' found"
                    it.children().each {
                      def datapoint_id
                      if ( it.name() == "parameter" ) {
                        //println "=====> datapoint '${channel_type}|${it.@id}' found"
                        datapoint_id = it.@id.toString()
                        //println datapoint_id
                        data_points_translation_map[it.@id]=["type":"channel","channel_type":channel_type,"stringTable_name":"stringTableHB"+"_"+creator_prefix+"_"+datapoint_id,"options":[]]
                      }
                      // check for options
                      it.children().each {
                        if ( ( it.name() == "logical" ) && ( it.@type == "option" ) ) {
                          it.children().each {
                            //println "=====>  option '${datapoint_id}=${it.@id}'"
                            my_option = master_parameter_id + "=" + it.@id.toString()
                            //println my_option
                            data_points_translation_map[datapoint_id]."options".add(my_option)
                            //println data_points_translation_map[datapoint_id]
                          }
                        }
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
                def keyset_id = it.@id.toString()

                it.children().each {
                  if (it.name() == "parameter" )
                  { 
                    //println "===== KEY of '${keyset_id}': 'KEY|${it.@id}' found"
                    def key_id = it.@id.toString()
                    def my_operations = it.@operations.toString()
                    if ( it.@ui_flags != "internal" ) {
                      //println "KEY: " + key_id
                      keys_translation_map[key_id]=["type":"param_defs","stringTable_name":"stringTableHB"+"_"+creator_prefix+"_"+key_id,"options":[], "operations":my_operations]
                      //println keys_translation_map[key_id]
                      it.children().each {
                        if ( ( it.name() == "logical" ) && ( it.@type == "option" ) ) {
                          it.children().each {
                            //println "=====>  option 'KEY|${key_id}': '${it.@id}'"
                            def my_key_option = key_id + "=" + it.@id.toString()
                            keys_translation_map[key_id]."options".add(my_key_option)
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
        
        println "\n\nExtracted master parameter candidates for translation are:\n\n"
        
        master_parameter_translation_map.each{
          print "=====>"
          print it
          println (" : " + master_parameter_translation_map[it])
        }
        
        
        println "\n\nExtracted data point candidates for translation are:\n\n"
        
        data_points_translation_map.each{
          print "=====>"
          print it
          println (" : " + data_points_translation_map[it])
        }
        
        println "\n\nExtracted key_id candidates for translation are:\n\n"
        
        keys_translation_map.each{
          print "=====>"
          print it
          println (" : " + keys_translation_map[it])
        }
        
        def my_file_name
        my_file_name = check_file("webui.js")
        def webui_js_map                   =  parse_webui_js(my_file_name)
        my_file_name = check_file("stringtable_de.txt")
        def StringTable_de_map             =  parseStringTable(my_file_name)
        my_file_name = check_file("translate.lang.stringtable.js")
        def translate_lang_stringtable_map = parseJson(my_file_name)
        
        println "\n.. removing duplicated entries .. \n\n"
        
        master_parameter_translation_map = remove_duplicates_from_translation_map(master_parameter_translation_map, webui_js_map, StringTable_de_map, translate_lang_stringtable_map)
        
        data_points_translation_map = remove_duplicates_from_translation_map(data_points_translation_map, webui_js_map, StringTable_de_map, translate_lang_stringtable_map)
        
        keys_translation_map = remove_duplicates_from_translation_map(keys_translation_map, webui_js_map, StringTable_de_map, translate_lang_stringtable_map)
        
        println "\n\nRemaining master parameter candidates for translation are:\n\n"
        
        master_parameter_translation_map.each{
          print "=====>"
          print it
          println (" : " + master_parameter_translation_map[it])
        }
        
        println "\n\nRemaining data point candidates for translation are:\n\n"
        
        data_points_translation_map.each{
          print "=====>"
          print it
          println (" : " + data_points_translation_map[it])
        }
        
        println "\n\nRemaining key_id candidates for translation are:\n\n"
        
        keys_translation_map.each{
          print "=====>"
          print it
          println (" : " + keys_translation_map[it])
        }
        
        save_map_to_file(data_points_translation_map, Addon_translation_archive_file)
        
        def restored_data_points_translation_map = read_map_from_file(Addon_translation_archive_file)
        
        println "\nRestored map\n"
        println restored_data_points_translation_map
        
        create_map_for_xml_generation(VersionString, device_name, device_id)
      }
    }
