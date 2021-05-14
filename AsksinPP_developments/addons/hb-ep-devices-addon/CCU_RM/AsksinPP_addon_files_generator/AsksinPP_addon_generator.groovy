#!/usr/bin/env groovy

package AsksinPP_addon_generator
/**
* A Simple example that parses an XML stored in a file using XmlSlurper.
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


class Addon_generator {

    static VersionString = "0.94"
    
    static def String my_encodeAsHTML (String input_string) {

        def my_html4_encoding_map = [
        
            "À":"\\&Agrave;",
            "Á":"\\&Aacute;",
            "Â":"\\&Acirc;",
            "Ã":"\\&Atilde;",
            "Ä":"\\&Auml;",
            "Å":"\\&Aring;",
            "Æ":"\\&AElig;",
            "Ç":"\\&Ccedil;",
            "È":"\\&Egrave;",
            "É":"\\&Eacute;",
            "Ê":"\\&Ecirc;",
            "Ë":"\\&Euml;",
            "Ì":"\\&Igrave;",
            "Í":"\\&Iacute;",
            "Î":"\\&Icirc;",
            "Ï":"\\&Iuml;",
            "Ð":"\\&ETH;",
            "Ñ":"\\&Ntilde;",
            "Ò":"\\&Ograve;",
            "Ó":"\\&Oacute;",
            "Ô":"\\&Ocirc;",
            "Õ":"\\&Otilde;",
            "Ö":"\\&Ouml;",
            "Ø":"\\&Oslash;",
            "Ù":"\\&Ugrave;",
            "Ú":"\\&Uacute;",
            "Û":"\\&Ucirc;",
            "Ü":"\\&Uuml;",
            "Ý":"\\&Yacute;",
            "Þ":"\\&THORN;",
            "ß":"\\&szlig;",
            "à":"\\&agrave;",
            "á":"\\&aacute;",
            "â":"\\&acirc;",
            "ã":"\\&atilde;",
            "ä":"\\&auml;",
            "å":"\\&aring;",
            "æ":"\\&aelig;",
            "ç":"\\&ccedil;",
            "è":"\\&egrave;",
            "é":"\\&eacute;",
            "ê":"\\&ecirc;",
            "ë":"\\&euml;",
            "ì":"\\&igrave;",
            "í":"\\&iacute;",
            "î":"\\&icirc;",
            "ï":"\\&iuml;",
            "ð":"\\&eth;",
            "ñ":"\\&ntilde;",
            "ò":"\\&ograve;",
            "ó":"\\&oacute;",
            "ô":"\\&ocirc;",
            "õ":"\\&otilde;",
            "ö":"\\&ouml;",
            "ø":"\\&oslash;",
            "ù":"\\&ugrave;",
            "ú":"\\&uacute;",
            "û":"\\&ucirc;",
            "ü":"\\&uuml;",
            "ý":"\\&yacute;",
            "þ":"\\&thorn;",
            "ÿ":"\\&yuml;",
            "/":"\\/"
            
        ]

        def encoded_string = ""

        input_string.each { c ->
            if ( my_html4_encoding_map.containsKey(c) ) {
            encoded_string += my_html4_encoding_map.get(c)
            }
            else {
            encoded_string += c
            }
        } 
        return encoded_string
    }
    
    static def check_control_XML_file_name(String control_file_name) {
       
        println "\nThe specified XML control file name is :  '${control_file_name}'"
        def file = new File(control_file_name)
        boolean isFile =      file.isFile(); 

        if ( ! file.exists() ){
        println "The file '${control_file_name}' is NOT existing"
        println "Aborting .."
        System.exit(0)
        }

        if ( ! file.isFile() ){
        println "The file '${control_file_name}' is NOT a regular file"
        println "Aborting .."
        System.exit(0)
        }

        if ( ! file.canRead() ){
        println "The file '${control_file_name}' is NOT readable"
        println "Aborting .."
        System.exit(0)
        }

        def control_file_name_extension = control_file_name.lastIndexOf('.').with {it != -1 ? control_file_name.substring(it+1):''}

        if ( control_file_name_extension != 'xml' ) {
        println "The extension of '${control_file_name}' is not 'xml': '${control_file_name_extension}'"
        println "Aborting .."
        System.exit(0)
        }

        try {
        File fXmlFile = new File(control_file_name)
        DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance()
        DocumentBuilder dBuilder = dbFactory.newDocumentBuilder()
        Document doc = dBuilder.parse(fXmlFile)
        } catch(Exception ex) {
            println (" ")
            println("The specified XML control file '${control_file_name}' is not a well-formed XML file")
            println (" ")
            println "Aborting .."
            /*throw new RuntimeException("The specified XML control file '${control_file_name}' is not a well-formed XML file", ex)*/
            System.exit(0)
        } 
        
        return control_file_name
    }

    static def get_control_XML_file_name() {
    
        print "Please input the file name of the AsksinPP addon generator XML control file : "
        def control_file_name = System.in.newReader().readLine()
        
        def checked_control_file_name = check_control_XML_file_name(control_file_name)
        
        return checked_control_file_name
    } 
    

    static void main(String... args) {
        
        println "\nAsksinPP addon script generator ${VersionString}\n\n"
        
        def my_xml_control_file_name
     
        if ( args.size() == 0 ) {
          my_xml_control_file_name = get_control_XML_file_name()
        }
        
        if ( args.size() == 1 ) {
          my_xml_control_file_name = check_control_XML_file_name(args[0])
        }
        
        if ( args.size() > 1 ) {
          println "Too many command line parameters !"
          println "\nUsage: groovy AsksinPP_addon_generator.groovy [<xml_control_file_name>]\n"
          println "Aborting .."
          System.exit(0)
        }
        
        println "XML control file : ${my_xml_control_file_name}"
        
        def AsksinPP_addon_control_file = new XmlSlurper().parse(new File(my_xml_control_file_name))
        
        String.metaClass.encodeURL = {
            java.net.URLEncoder.encode(delegate, "ISO8859-1").replace("+", "%20")
        }
        
        def String my_channel_type = ""
        
        println ""
        println "addon name                     : ${AsksinPP_addon_control_file.addon_name}"
        println "addon version                  : ${AsksinPP_addon_control_file.addon_version}"
        println "creator                        : ${AsksinPP_addon_control_file.creator}"
        println "creator prefix                 : ${AsksinPP_addon_control_file.creator_prefix}"
        println "created_on                     : ${AsksinPP_addon_control_file.created_on}"
        println "extract_rftypes_XML version    : ${AsksinPP_addon_control_file.generator_version}"
        
 
        def install_script_file_name    = "install_"+"${AsksinPP_addon_control_file.addon_name}"
        println "install script name    : ${install_script_file_name}"

        def uninstall_script_file_name  = "uninstall_"+"${AsksinPP_addon_control_file.addon_name}"
        println "uninstall script name  : ${uninstall_script_file_name}"
        
        
        
        //============================================================================================================================================
        // generate install script
        //============================================================================================================================================
        
        println "\n\n.. creating install script '${install_script_file_name}' ..\n\n"
        
        
        File isf = new File(install_script_file_name)
        def  webuiInsert_buffer                   = StringBuffer.newInstance()
        def  stringtable_de_txt_buffer            = StringBuffer.newInstance()
        def  translate_lang_stringtable_js_buffer = StringBuffer.newInstance()
        
        isf.write  "#!/bin/sh\n\n"
        isf.append "#------------------------------------------------------------------------------\n"
        isf.append "#------------------------------------------------------------------------------\n"
        isf.append "# addon name                  : ${AsksinPP_addon_control_file.addon_name}\n"
        isf.append "# addon version               : ${AsksinPP_addon_control_file.addon_version}\n#\n"
        isf.append "# generated by AsksinPP_addon_generator ${VersionString}\n"
        def my_date = new Date()
        isf.append "# generated on ${my_date}\n\n"
        isf.append "# XML control file name       : ${my_xml_control_file_name}\n\n"
        isf.append "# creator                     : ${AsksinPP_addon_control_file.creator}\n"
        isf.append "# created_on                  : ${AsksinPP_addon_control_file.created_on}\n"
        isf.append "# extract_rftypes_XML version : ${AsksinPP_addon_control_file.generator_version}\n"
        isf.append "#------------------------------------------------------------------------------\n"
        isf.append "#------------------------------------------------------------------------------\n"
        
        isf.append "# global settings for editing DEVDB.tcl\n"
        isf.append "devdescrFile=\"/www/config/devdescr/DEVDB.tcl\"\n"
        isf.append "devdescrSearch=\"array[[:space:]]*set[[:space:]]*DEV_PATHS[[:space:]]*{\"\n\n"
        isf.append "#------------------------------------------------------------------------------\n"
        isf.append "#------------------------------------------------------------------------------\n"
        
        isf.append "# global settings for editing webui.js\n"
        isf.append "webuiFile=\"/www/webui/webui.js\"\n"
        isf.append "webuiSearch=\"DEV_HIGHLIGHT[[:space:]]*=[[:space:]]*new Array();\"\n\n\n"
        isf.append "#------------------------------------------------------------------------------\n"
        isf.append "#------------------------------------------------------------------------------\n"
        isf.append "# iterate over the addon devices\n"
        isf.append "#------------------------------------------------------------------------------\n"
        isf.append "#------------------------------------------------------------------------------\n\n"
        
        
        AsksinPP_addon_control_file.devices.device.each {
        
          println "\n.. working on device '${it.@id}' ..\n\n"
          println "device id              : ${it.@id}"
          println "device description     : ${it.device_description}"
          println "small case device name : ${it.@device_small_case_name}"
          println "XML version            : ${it.@rftypes_XML_version}"
          println "index                  : ${it.@index}"
          
          
          isf.append "#------------------------------------------------------------------------------\n"
          isf.append "#------------------------------------------------------------------------------\n"
          isf.append "# device name            : ${it.@id}\n"
          isf.append "# small case device name : ${it.@device_small_case_name}\n"
          isf.append "# XML version            : ${it.@rftypes_XML_version}\n"
          isf.append "# index                  : ${it.@index}\n"
          isf.append "#------------------------------------------------------------------------------\n"
          isf.append "#------------------------------------------------------------------------------\n"
          isf.append "# in der WebUI angezeigter Gerätetyp, muss identisch sein mit dem Firmware-XML-Tag: <type name=\"${it.@id}\" id=\"${it.@id}\">\n"
          isf.append "DEVICE=\"${it.@id}\"\n"
          isf.append "# in der WebUI angezeigte Gerätebeschreibung\n"
          String toEncode = "${it.device_description}".toString()
          toEncode = my_encodeAsHTML(toEncode)
          isf.append "DEVICE_DESC=\"${toEncode}\"\n"

          isf.append "# Name der Piktogramme, bestehend aus xx.png bzw. xx_thumb.png\n"
          isf.append "DEVICE_IMG=${it.@device_small_case_name}.png\n"
          isf.append "DEVICE_THUMB=${it.@device_small_case_name}_thumb.png\n"
          isf.append "FIRMWARE_FILE=/firmware/rftypes/${it.@device_small_case_name}*.xml\n\n"

          isf.append "# edit DEVDB.tcl\n"
          isf.append "devdescrInsert=\"\$DEVICE {{50 \\/config\\/img\\/devices\\/50\\/\$DEVICE_THUMB} {250 \\/config\\/img\\/devices\\/250\\/\$DEVICE_IMG}} \"\n\n"

          isf.append "if [ -z \"`cat \$devdescrFile | grep \\\"\$DEVICE\\\"`\" ]; then\n"
          isf.append "	sed -i \"s/\\(\$devdescrSearch\\)/\\1\$devdescrInsert/g\" \$devdescrFile\n"
          isf.append "fi\n\n"

          isf.append "# edit webui.js\n"
          isf.append "webuiInsert=\"\\n\"\n"
          isf.append "webuiInsert=\"\${webuiInsert}DEV_HIGHLIGHT['\$DEVICE'] = new Object();\\n\"\n"
          isf.append "webuiInsert=\"\${webuiInsert}DEV_LIST.push('\$DEVICE');\\n\"\n"
          isf.append "webuiInsert=\"\${webuiInsert}DEV_DESCRIPTION['\$DEVICE']='\$DEVICE_DESC';\\n\"\n"
          isf.append "webuiInsert=\"\${webuiInsert}DEV_PATHS['\$DEVICE'] = new Object();\\n\"\n"
          isf.append "webuiInsert=\"\${webuiInsert}DEV_PATHS['\$DEVICE']['50'] = '\\/config\\/img\\/devices\\/50\\/\$DEVICE_THUMB';\\n\"\n"
          isf.append "webuiInsert=\"\${webuiInsert}DEV_PATHS['\$DEVICE']['250'] = '\\/config\\/img\\/devices\\/250\\/\$DEVICE_IMG';\"\n\n"
          
          isf.append "if [ -z \"`cat \$webuiFile | grep \\\"\$DEVICE\\\"`\" ]; then\n"
          isf.append "	sed -i \"s/\\(\$webuiSearch\\)/\\1\$webuiInsert/g\" \$webuiFile\n"
          isf.append "fi\n\n"
        }

        isf.append "#------------------------------------------------------------------------------\n"
        isf.append "#------------------------------------------------------------------------------\n"
        isf.append "#------------------------------------------------------------------------------\n"
        isf.append "# ab hier siehe Anleitung von Jérôme, siehe https://homematic-forum.de/forum/viewtopic.php?f=76&t=60067\n"
        isf.append "#------------------------------------------------------------------------------\n"
        isf.append "#------------------------------------------------------------------------------\n"
        isf.append "#------------------------------------------------------------------------------\n\n\n"

        isf.append "webuiSearchBegin=\"elvST[[:space:]]*=[[:space:]]*new Array();\"\n\n"


        
        webuiInsert_buffer.append( "\n\n\n### Edit webuiInsert ####\n\n" )
        
        webuiInsert_buffer.append( "webuiFile=\"/www/webui/webui.js\"\n" )
        
        stringtable_de_txt_buffer.append( "\n\n\n### Edit stringtable_de.txt ###\n\n" )
        stringtable_de_txt_buffer.append( "stringtable_deFile=\"/www/config/stringtable_de.txt\"\n" )
        
        translate_lang_stringtable_js_buffer.append( "\n\n\n### Edit translate.lang.stringtable.js ###\n\n" )
        translate_lang_stringtable_js_buffer.append( "translate_deFile=\"/www/webui/js/lang/de/translate.lang.stringtable.js\"\n" )
        translate_lang_stringtable_js_buffer.append( "translate_deSearch=\"\\\"dummy\\\" : \\\"\\\",\"\n\n" )
        
        AsksinPP_addon_control_file.paramset.parameter.each
        {
          
          println "\n\n.. working on paramset '${it.@paramset_type}' '${it.@paramset_id}' .. \n"
          println " "
          println "index                  : ${it.@index}"
          println "parameter id           : ${it.@id}"
          println "paramset type          : ${it.@paramset_type}"
          println "paramset id            : ${it.@paramset_id}"

          String toEncode = "${it.translation_de}"
          println "translated_string      : ${toEncode.encodeURL()}"
          println "stringtable_identifier : ${it.stringtable_identifier}"
         
          
          webuiInsert_buffer.append( " \n" )
          stringtable_de_txt_buffer.append( " \n" )
          translate_lang_stringtable_js_buffer.append( " \n" )
          
          webuiInsert_buffer.append("\n# parameter id #${it.@index}: ${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} \n")
          webuiInsert_buffer.append( "  \n")
          webuiInsert_buffer.append( "webuiInsertParam=\"${it.@id}\"\n" )
          webuiInsert_buffer.append( "webuiInsertValue=\"${it.stringtable_identifier}\"\n" )
          webuiInsert_buffer.append( "webuiInsert=\"\\n\"\n" )
          webuiInsert_buffer.append( "webuiInsert=\"\${webuiInsert}elvST['\$webuiInsertParam'] = '\\\${\$webuiInsertValue}';\"\n" )
          webuiInsert_buffer.append( "if [ -z \"`cat \$webuiFile | grep \\\"\$webuiInsertParam\\\"`\" ]; then\n" )
          webuiInsert_buffer.append( "	sed -i \"s/\\(\$webuiSearchBegin\\)/\\1\$webuiInsert/g\" \$webuiFile\n" )
          webuiInsert_buffer.append( "fi\n" )
          webuiInsert_buffer.append( " \n" )
          webuiInsert_buffer.append( " \n" )
          
          
          
          stringtable_de_txt_buffer.append("\n# parameter id #${it.@index}: ${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} \n")
          stringtable_de_txt_buffer.append( " \n" )
          stringtable_de_txt_buffer.append( "stringtable_deInsert=\"${it.@id}\\t\\\${${it.stringtable_identifier}}\"\n" )
          stringtable_de_txt_buffer.append( "if [ -z \"`cat \$stringtable_deFile | grep \\\"${it.@id}\\\"`\" ]; then\n" )
          stringtable_de_txt_buffer.append( "    echo -e \$stringtable_deInsert >> \$stringtable_deFile\n" )
          stringtable_de_txt_buffer.append( "fi\n" )
          
          
          
          translate_lang_stringtable_js_buffer.append("\n# parameter id #${it.@index}: ${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} \n")
          translate_lang_stringtable_js_buffer.append( " \n" )
          toEncode = "${it.translation_de}"
          translate_lang_stringtable_js_buffer.append( "translate_deInsert=\"\\n    \\\"${it.stringtable_identifier}\\\" : \\\"${toEncode.encodeURL()}\\\",\"\n" )
          translate_lang_stringtable_js_buffer.append( "if [ -z \"`cat \$translate_deFile | grep \\\"${it.stringtable_identifier}\\\"`\" ]; then\n" )
          translate_lang_stringtable_js_buffer.append( "      sed -i \"s/\\(\$translate_deSearch\\)/\\1\$translate_deInsert/g\" \$translate_deFile\n" )
          translate_lang_stringtable_js_buffer.append( "fi\n" )
          translate_lang_stringtable_js_buffer.append( " \n" )
          
        }
        
        AsksinPP_addon_control_file.channel.parameter.each
        {
          
          println "\n\n.. working on channel '${it.@channel}' .. \n"
          println " "
          println "index                  : ${it.@index}"
          println "parameter id           : ${it.@id}"
          println "paramset type          : ${it.@paramset_type}"
          println "paramset id            : ${it.@paramset_id}"
          println "channel                : ${it.@channel}"

          String toEncode = "${it.translation_de}"
          println "translated_string      : ${toEncode.encodeURL()}"
          println "stringtable_identifier : ${it.stringtable_identifier}"

          def my_channel_prefix
          if ( it.skip_channel_prefix == "no" ) {
            my_channel_prefix = "${it.@channel}|"
          }
          else {
            my_channel_prefix = ""
          }
          
          webuiInsert_buffer.append( " \n" )
          stringtable_de_txt_buffer.append( " \n" )
          translate_lang_stringtable_js_buffer.append( " \n" )
          
          webuiInsert_buffer.append("\n# parameter id #${it.@index}: ${my_channel_prefix}${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} channel ${it.@channel} \n")
          webuiInsert_buffer.append( " \n" )
          webuiInsert_buffer.append( "webuiInsertParam=\"${my_channel_prefix}${it.@id}\"\n" )
          webuiInsert_buffer.append( "webuiInsertValue=\"${it.stringtable_identifier}\"\n" )
          webuiInsert_buffer.append( "webuiInsert=\"\\n\"\n" )
          webuiInsert_buffer.append( "webuiInsert=\"\${webuiInsert}elvST['\$webuiInsertParam'] = '\\\${\$webuiInsertValue}';\"\n" )
          webuiInsert_buffer.append( "if [ -z \"`cat \$webuiFile | grep \\\"\$webuiInsertParam\\\"`\" ]; then\n" )
          webuiInsert_buffer.append( "	sed -i \"s/\\(\$webuiSearchBegin\\)/\\1\$webuiInsert/g\" \$webuiFile\n" )
          webuiInsert_buffer.append( "fi\n" )
          webuiInsert_buffer.append( " \n" )
          webuiInsert_buffer.append( " \n" )
          
          
          
          stringtable_de_txt_buffer.append("\n#  parameter id #${it.@index}: ${my_channel_prefix}${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} channel ${it.@channel} \n")
          stringtable_de_txt_buffer.append( " \n" )
          stringtable_de_txt_buffer.append( "stringtable_deInsert=\"${my_channel_prefix}${it.@id}\\t\\\${${it.stringtable_identifier}}\"\n" )
          stringtable_de_txt_buffer.append( "if [ -z \"`cat \$stringtable_deFile | grep \\\"${my_channel_prefix}${it.@id}\\\"`\" ]; then\n" )
          stringtable_de_txt_buffer.append( "    echo -e \$stringtable_deInsert >> \$stringtable_deFile\n" )
          stringtable_de_txt_buffer.append( "fi\n" )
          
          
          
          translate_lang_stringtable_js_buffer.append("\n#  parameter id #${it.@index}: ${my_channel_prefix}${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} channel ${it.@channel} \n")
          translate_lang_stringtable_js_buffer.append( " \n" )
          toEncode = "${it.translation_de}"
          translate_lang_stringtable_js_buffer.append( "translate_deInsert=\"\\n    \\\"${it.stringtable_identifier}\\\" : \\\"${toEncode.encodeURL()}\\\",\"\n" )
          translate_lang_stringtable_js_buffer.append( "if [ -z \"`cat \$translate_deFile | grep \\\"${it.stringtable_identifier}\\\"`\" ]; then\n" )
          translate_lang_stringtable_js_buffer.append( "      sed -i \"s/\\(\$translate_deSearch\\)/\\1\$translate_deInsert/g\" \$translate_deFile\n" )
          translate_lang_stringtable_js_buffer.append( "fi\n" )
          translate_lang_stringtable_js_buffer.append( " \n" )
          
        }
        
        AsksinPP_addon_control_file.paramset_defs.parameter.each
        {
          
          println "\n\n.. working on paramset_defs .. \n"
          println " "
          println "index                  : ${it.@index}"
          println "parameter id           : ${it.@id}"
          println "paramset id            : ${it.@paramset_id}"


          toEncode = "${it.translation_de}"
          println "translated_string      : ${toEncode.encodeURL()}"
          println "stringtable_identifier : ${it.stringtable_identifier}"
          
          def my_key_prefix
          if ( it.skip_key_prefix == "yes" ) {
            my_key_prefix = ""
          }
          else {
            my_key_prefix = "${it.key_prefix}|"
          }
          
          webuiInsert_buffer.append( " \n" )
          stringtable_de_txt_buffer.append( " \n" )
          translate_lang_stringtable_js_buffer.append( " \n" )
          
          webuiInsert_buffer.append("\n# parameter id #${it.@index}: ${my_key_prefix}${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} \n")
          webuiInsert_buffer.append( " \n" )
          webuiInsert_buffer.append( "webuiInsertParam=\"${my_key_prefix}${it.@id}\"\n" )
          webuiInsert_buffer.append( "webuiInsertValue=\"${it.stringtable_identifier}\"\n" )
          webuiInsert_buffer.append( "webuiInsert=\"\\n\"\n" )
          webuiInsert_buffer.append( "webuiInsert=\"\${webuiInsert}elvST['\$webuiInsertParam'] = '\\\${\$webuiInsertValue}';\"\n" )
          webuiInsert_buffer.append( "if [ -z \"`cat \$webuiFile | grep \\\"\$webuiInsertParam\\\"`\" ]; then\n" )
          webuiInsert_buffer.append( "	sed -i \"s/\\(\$webuiSearchBegin\\)/\\1\$webuiInsert/g\" \$webuiFile\n" )
          webuiInsert_buffer.append( "fi\n" )
          webuiInsert_buffer.append( " \n" )
          webuiInsert_buffer.append( " \n" )
          
          
          
          stringtable_de_txt_buffer.append("\n# parameter id #${it.@index}: ${my_key_prefix}${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} \n")
          stringtable_de_txt_buffer.append( " \n" )
          stringtable_de_txt_buffer.append( "stringtable_deInsert=\"${my_key_prefix}${it.@id}\\t\\\${${it.stringtable_identifier}}\"\n" )
          stringtable_de_txt_buffer.append( "if [ -z \"`cat \$stringtable_deFile | grep \\\"${my_key_prefix}${it.@id}\\\"`\" ]; then\n" )
          stringtable_de_txt_buffer.append( "    echo -e \$stringtable_deInsert >> \$stringtable_deFile\n" )
          stringtable_de_txt_buffer.append( "fi\n" )
          
          
          
          translate_lang_stringtable_js_buffer.append("\n# parameter id #${it.@index}: ${my_key_prefix}${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} \n")
          translate_lang_stringtable_js_buffer.append( " \n" )
          toEncode = "${it.translation_de}"
          translate_lang_stringtable_js_buffer.append( "translate_deInsert=\"\\n    \\\"${it.stringtable_identifier}\\\" : \\\"${toEncode.encodeURL()}\\\",\"\n" )
          translate_lang_stringtable_js_buffer.append( "if [ -z \"`cat \$translate_deFile | grep \\\"${it.stringtable_identifier}\\\"`\" ]; then\n" )
          translate_lang_stringtable_js_buffer.append( "      sed -i \"s/\\(\$translate_deSearch\\)/\\1\$translate_deInsert/g\" \$translate_deFile\n" )
          translate_lang_stringtable_js_buffer.append( "fi\n" )
          translate_lang_stringtable_js_buffer.append( " \n" )
          
        }
        
        isf.append(webuiInsert_buffer)
        isf.append(stringtable_de_txt_buffer)
        isf.append(translate_lang_stringtable_js_buffer)
        isf.append("\n\n#finished 'AsksinPP addon script generator ${VersionString}'\n")
        
        
        
        
    
        //============================================================================================================================================
        // generate uninstall script
        //============================================================================================================================================
        
        
        println "\n\n.. creating uninstall script '${uninstall_script_file_name}' ..\n\n"
        
        File uisf = new File(uninstall_script_file_name)
        
        webuiInsert_buffer                   = webuiInsert_buffer.delete(0, webuiInsert_buffer.length())
        stringtable_de_txt_buffer            = stringtable_de_txt_buffer.delete(0, stringtable_de_txt_buffer.length())
        translate_lang_stringtable_js_buffer = translate_lang_stringtable_js_buffer.delete(0, translate_lang_stringtable_js_buffer.length())

        
        uisf.write  "#!/bin/sh\n\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        uisf.append "# addon name                  : ${AsksinPP_addon_control_file.addon_name}\n"
        uisf.append "# addon version               : ${AsksinPP_addon_control_file.addon_version}\n#\n"
        uisf.append "# generated by AsksinPP_addon_generator ${VersionString}\n"
        my_date = new Date()
        uisf.append "# generated on ${my_date}\n\n"
        uisf.append "# XML control file name       : ${my_xml_control_file_name}\n\n"
        uisf.append "# creator                     : ${AsksinPP_addon_control_file.creator}\n"
        uisf.append "# created_on                  : ${AsksinPP_addon_control_file.created_on}\n"
        uisf.append "# extract_rftypes_XML version : ${AsksinPP_addon_control_file.generator_version}\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        
        uisf.append "# global settings for editing DEVDB.tcl\n"
        uisf.append "devdescrFile=\"/www/config/devdescr/DEVDB.tcl\"\n"
        uisf.append "devdescrSearch=\"array[[:space:]]*set[[:space:]]*DEV_PATHS[[:space:]]*{\"\n\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        
        uisf.append "# global settings for editing webui.js\n"
        uisf.append "webuiFile=\"/www/webui/webui.js\"\n"
        uisf.append "webuiSearch=\"DEV_HIGHLIGHT[[:space:]]*=[[:space:]]*new Array();\"\n\n\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        uisf.append "# iterate over the addon devices\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        uisf.append "#------------------------------------------------------------------------------\n\n"
        
        AsksinPP_addon_control_file.devices.device.each {
        
          println "\n.. working on device '${it.@id}' ..\n\n"
          println "device id              : ${it.@id}"
          println "device description     : ${it.device_description}"
          println "small case device name : ${it.@device_small_case_name}"
          println "XML version            : ${it.@rftypes_XML_version}"
          println "index                  : ${it.@index}"
          
          
          uisf.append "#------------------------------------------------------------------------------\n"
          uisf.append "#------------------------------------------------------------------------------\n"
          uisf.append "# device name            : ${it.@id}\n"
          uisf.append "# small case device name : ${it.@device_small_case_name}\n"
          uisf.append "# XML version            : ${it.@rftypes_XML_version}\n"
          uisf.append "# index                  : ${it.@index}\n"
          uisf.append "#------------------------------------------------------------------------------\n"
          uisf.append "#------------------------------------------------------------------------------\n"
          uisf.append "# in der WebUI angezeigter Gerätetyp, muss identisch sein mit dem Firmware-XML-Tag: <type name=\"${it.@id}\" id=\"${it.@id}\">\n"
          uisf.append "DEVICE=\"${it.@id}\"\n"
          uisf.append "# in der WebUI angezeigte Gerätebeschreibung\n"
          String toEncode = "${it.device_description}".toString()
          toEncode = my_encodeAsHTML(toEncode)
          uisf.append "DEVICE_DESC=\"${toEncode}\"\n"

          uisf.append "# Name der Piktogramme, bestehend aus xx.png bzw. xx_thumb.png\n"
          uisf.append "DEVICE_IMG=${it.@device_small_case_name}.png\n"
          uisf.append "DEVICE_THUMB=${it.@device_small_case_name}_thumb.png\n"
          uisf.append "FIRMWARE_FILE=/firmware/rftypes/${it.@device_small_case_name}*.xml\n\n"
          
          uisf.append "#------------------------------------------------------------------------------\n"
          uisf.append "# Undo DEVDB.tcl changes\n"
          uisf.append "devdescrSearch=\"\$DEVICE {{50 \\/config\\/img\\/devices\\/50\\/\$DEVICE_THUMB} {250 \\/config\\/img\\/devices\\/250\\/\$DEVICE_IMG}} \"\n\n"

          uisf.append "if [ \"`cat \$devdescrFile | grep \\\"\$DEVICE\\\"`\" ]; then\n"
          uisf.append "        sed -i \"s/\\(\$devdescrSearch\\)//g\" \$devdescrFile\n"
          uisf.append "fi\n\n"

          uisf.append "#------------------------------------------------------------------------------\n"
          uisf.append "# Undo webui.js changes\n"
          uisf.append "if [ \"`cat \$webuiFile | grep \\\"\$DEVICE\\\"`\" ]; then\n"
          uisf.append "        sed -i \"/\\(\$DEVICE\\)/d\" \$webuiFile\n"
          uisf.append "fi\n\n"
          
          uisf.append "#------------------------------------------------------------------------------\n"
          uisf.append "# remove device_xml links\n"
          uisf.append "rm -f \$FIRMWARE_FILE\n\n"
          
          uisf.append "#------------------------------------------------------------------------------\n"
          uisf.append "# remove thumb files\n"
          uisf.append "rm -f /www/config/img/devices/250/\$DEVICE_IMG\n"
          uisf.append "rm -f /www/config/img/devices/50/\$DEVICE_THUMB\n\n"
        }

        uisf.append "#------------------------------------------------------------------------------\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        uisf.append "# ab hier siehe Anleitung von Jérôme, siehe https://homematic-forum.de/forum/viewtopic.php?f=76&t=60067\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        uisf.append "#------------------------------------------------------------------------------\n"
        uisf.append "#------------------------------------------------------------------------------\n\n\n"
        
        
        
    

        
        webuiInsert_buffer.append( "\n\n\n### Undo changes of webuiFile ####\n\n" )
        
        
        stringtable_de_txt_buffer.append( "\n\n\n### Undo changes of stringtable_de.txt ###\n\n" )
        stringtable_de_txt_buffer.append( "stringtable_deFile=\"/www/config/stringtable_de.txt\"\n" )
        
        translate_lang_stringtable_js_buffer.append( "\n\n\n### Undo changes of translate.lang.stringtable.js ###\n\n" )
        translate_lang_stringtable_js_buffer.append( "translate_deFile=\"/www/webui/js/lang/de/translate.lang.stringtable.js\"\n" )
        
        AsksinPP_addon_control_file.paramset.parameter.each
        {
          
          println "\n\n.. working on paramset '${it.@paramset_type}' '${it.@paramset_id}' .. \n"
          println " "
          println "index                  : ${it.@index}"
          println "parameter id           : ${it.@id}"
          println "paramset type          : ${it.@paramset_type}"
          println "paramset id            : ${it.@paramset_id}"

          def toEncode = "${it.translation_de}"
          println "translated_string      : ${toEncode.encodeURL()}"
          println "stringtable_identifier : ${it.stringtable_identifier}"
          
          webuiInsert_buffer.append( " \n" )
          stringtable_de_txt_buffer.append( " \n" )
          translate_lang_stringtable_js_buffer.append( " \n" )
          
          webuiInsert_buffer.append("\n# parameter id #${it.@index}: ${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} \n")
          webuiInsert_buffer.append(" \n")
          webuiInsert_buffer.append( "webuiSearch=\"${my_channel_type}${it.@id}\"\n" )
          webuiInsert_buffer.append( "sed -i \"/\\(\$webuiSearch\\)/d\" \$webuiFile\n\n" )
          
          
          stringtable_de_txt_buffer.append("\n# parameter id #${it.@index}: ${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} \n")
          stringtable_de_txt_buffer.append( " \n" )
          stringtable_de_txt_buffer.append( "stringtable_deSearch=\"${it.@id}\"\n")
          stringtable_de_txt_buffer.append( "sed -i \"/\\(\$stringtable_deSearch\\)/d\" \$stringtable_deFile\n")

          
          
          translate_lang_stringtable_js_buffer.append("\n# parameter id #${it.@index}: ${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} \n")
          translate_lang_stringtable_js_buffer.append( " \n" )
          translate_lang_stringtable_js_buffer.append("translate_deSearch=\"${it.stringtable_identifier}\"\n")
          translate_lang_stringtable_js_buffer.append("sed -i \"/\\(\${translate_deSearch}\\)/d\" \$translate_deFile\n")
          
        }
        
        AsksinPP_addon_control_file.channel.parameter.each
        {
          
          println "\n\n.. working on channel '${it.@channel}' .. \n"
          println " "
          println "index                  : ${it.@index}"
          println "parameter id           : ${it.@id}"
          println "paramset type          : ${it.@paramset_type}"
          println "paramset id            : ${it.@paramset_id}"
          println "channel                : ${it.@channel}"

          def toEncode = "${it.translation_de}"
          println "translated_string      : ${toEncode.encodeURL()}"
          println "stringtable_identifier : ${it.stringtable_identifier}"

          def my_channel_prefix
          if ( it.skip_channel_prefix == "no" ) {
            my_channel_prefix = "${it.@channel}|"
          }
          else {
            my_channel_prefix = ""
          }
          
          webuiInsert_buffer.append( " \n" )
          stringtable_de_txt_buffer.append( " \n" )
          translate_lang_stringtable_js_buffer.append( " \n" )
          
          webuiInsert_buffer.append("\n# parameter id #${it.@index}: ${my_channel_prefix}${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} channel ${it.@channel} \n")
          webuiInsert_buffer.append(" \n")
          webuiInsert_buffer.append( "webuiSearch=\"${my_channel_prefix}${it.@id}\"\n" )
          webuiInsert_buffer.append( "sed -i \"/\\(\$webuiSearch\\)/d\" \$webuiFile\n\n" )
          
          
          stringtable_de_txt_buffer.append("\n# parameter id #${it.@index}: ${my_channel_prefix}${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} channel ${it.@channel} \n")
          stringtable_de_txt_buffer.append( " \n" )
          stringtable_de_txt_buffer.append( "stringtable_deSearch=\"${my_channel_prefix}${it.@id}\"\n")
          stringtable_de_txt_buffer.append( "sed -i \"/\\(\$stringtable_deSearch\\)/d\" \$stringtable_deFile\n")

          
          
          translate_lang_stringtable_js_buffer.append("\n# parameter id #${it.@index}: ${my_channel_prefix}${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} channel ${it.@channel} \n")
          translate_lang_stringtable_js_buffer.append( " \n" )
          translate_lang_stringtable_js_buffer.append("translate_deSearch=\"${it.stringtable_identifier}\"\n")
          translate_lang_stringtable_js_buffer.append("sed -i \"/\\(\${translate_deSearch}\\)/d\" \$translate_deFile\n")
          
        }
        
        AsksinPP_addon_control_file.paramset_defs.parameter.each
        {
          
          println "\n\n.. working on paramset_defs .. \n"
          println " "
          println "index                  : ${it.@index}"
          println "parameter id           : ${it.@id}"
          println "paramset id            : ${it.@paramset_id}"


          toEncode = "${it.translation_de}"
          println "translated_string      : ${toEncode.encodeURL()}"
          println "stringtable_identifier : ${it.stringtable_identifier}"
          
          def my_key_prefix
          if ( it.skip_key_prefix == "yes" ) {
            my_key_prefix = ""
          }
          else {
            my_key_prefix = "${it.key_prefix}|"
          }
          
          
          webuiInsert_buffer.append( " \n" )
          stringtable_de_txt_buffer.append( " \n" )
          translate_lang_stringtable_js_buffer.append( " \n" )
          
          webuiInsert_buffer.append("\n# parameter id #${it.@index}: ${my_key_prefix}${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} \n")
          webuiInsert_buffer.append(" \n")
          webuiInsert_buffer.append( "webuiSearch=\"${my_key_prefix}${it.@id}\"\n" )
          webuiInsert_buffer.append( "sed -i \"/\\(\$webuiSearch\\)/d\" \$webuiFile\n\n" )
          
          
          stringtable_de_txt_buffer.append("\n# parameter id #${it.@index}: ${my_key_prefix}${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} \n")
          stringtable_de_txt_buffer.append( " \n" )
          stringtable_de_txt_buffer.append( "stringtable_deSearch=\"${my_key_prefix}${it.@id}\"\n")
          stringtable_de_txt_buffer.append( "sed -i \"/\\(\$stringtable_deSearch\\)/d\" \$stringtable_deFile\n")

          
          
          translate_lang_stringtable_js_buffer.append("\n# parameter id #${it.@index}: ${my_key_prefix}${it.@id} paramset ${it.@paramset_type} ${it.@paramset_id} \n")
          translate_lang_stringtable_js_buffer.append( " \n" )
          translate_lang_stringtable_js_buffer.append("translate_deSearch=\"${it.stringtable_identifier}\"\n")
          translate_lang_stringtable_js_buffer.append("sed -i \"/\\(\${translate_deSearch}\\)/d\" \$translate_deFile\n")
          
        }
        
        uisf.append(webuiInsert_buffer)
        uisf.append(stringtable_de_txt_buffer)
        uisf.append(translate_lang_stringtable_js_buffer)
        uisf.append("\n\n#finished 'AsksinPP addon script generator ${VersionString}'\n")
        
        
        println "\n\n.. created install script '${install_script_file_name}'"
        println ".. created uninstall script '${uninstall_script_file_name}'\n"
        
        println "\nfinished 'AsksinPP addon script generator ${VersionString}'\n"
    }
    
    

}



