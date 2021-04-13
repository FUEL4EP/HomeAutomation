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




class extract_rftypes_XML {

    static VersionString = "0.10"
    
    static def check_control_XML_file_name(String rftypes_XML_file_name) {
       
        println "\nThe specified rftypes XML file name is :  '${rftypes_XML_file_name}'"
        def file = new File(rftypes_XML_file_name)
        boolean isFile =      file.isFile(); 

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
        
        xml.children().each {
          //println "1 '${it.name()}'"
          // parse node 'supported_types node'
          if (it.name() == "supported_types" )
          { //found node 'supported_types'
            println "=> supported_types found"
           
          }
          if (it.name() == "channels" )
          { // found node 'channels'
            println "=> channels found"
            it.children().each {
              if ( it.name() == "channel" ) {
                println "=> channel  '${it.@type}' found"
                def channel_type = it.@type
                it.children().each {
                  if (it.name() == "paramset" )
                  { println "=> paramset '${it.@type}' found"
                    it.children().each {
                      def datapoint_id
                      if ( it.name() == "parameter" ) {
                        println "=====> datapoint '${channel_type}|${it.@id}' found"
                        datapoint_id = it.@id
                      }
                      // check for option
                      it.children().each {
                        if ( ( it.name() == "logical" ) && ( it.@type == "option" ) ) {
                          it.children().each {
                            println "=====>  option '${datapoint_id}=${it.@id}'"

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
          { println "=> paramset_defs found"
            it.children().each {
              if (it.name() == "paramset" )
              { println "=> paramset '${it.@id}' found"
                def keyset_id = it.@id
                if ( it.@id == "key_paramset" ) {
                  it.children().each {
                    if (it.name() == "parameter" )
                    { 
                      println "===== KEY of '${keyset_id}': 'KEY|${it.@id}' found"
                      it.children().each {
                        if ( ( it.name() == "logical" ) && ( it.@type == "option" ) ) {
                          it.children().each {
                            println "=====>  option 'KEY|${it.@id}': '${it.@id}'"
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
        
        println "\n\nAll lines above marked by '=====>' are candidates for a translation"
        println "This will be done in a later version of this generator software\n\n"
        println "\nOpen tasks are: Scan 'webui.js', 'stringtable_de.txt', and 'translate.lang.stringtable.js' of EQ-3\'s 'occu' for existing entries\n"
      }
    }
