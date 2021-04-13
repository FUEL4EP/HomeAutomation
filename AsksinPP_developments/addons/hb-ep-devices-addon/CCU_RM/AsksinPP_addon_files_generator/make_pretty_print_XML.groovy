

package read_rftypes_XML
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
import groovy.xml.XmlUtil
import groovy.util.Node




class pretty_print_XML {

    static VersionString = "0.10"
    
    static def check_input_XML_file_name(String input_XML_file_name) {
       
        println "\nThe specified input XML file name is   : '${input_XML_file_name}'"
        def file = new File(input_XML_file_name)
        boolean isFile =      file.isFile(); 

        if ( ! file.exists() ){
        println "The file '${input_XML_file_name}' is NOT existing"
        println "Aborting .."
        System.exit(0)
        }

        if ( ! file.isFile() ){
        println "The file '${input_XML_file_name}' is NOT a regular file"
        println "Aborting .."
        System.exit(0)
        }

        if ( ! file.canRead() ){
        println "The file '${input_XML_file_name}' is NOT readable"
        println "Aborting .."
        System.exit(0)
        }

        def input_XML_file_name_extension = input_XML_file_name.lastIndexOf('.').with {it != -1 ? input_XML_file_name.substring(it+1):''}

        if ( input_XML_file_name_extension != 'xml' ) {
        println "The extension of '${input_XML_file_name}' is not 'xml': '${input_XML_file_name_extension}'"
        println "Aborting .."
        System.exit(0)
        }

        try {
        File fXmlFile = new File(input_XML_file_name)
        DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance()
        DocumentBuilder dBuilder = dbFactory.newDocumentBuilder()
        Document doc = dBuilder.parse(fXmlFile)
        } catch(Exception ex) {
            println (" ")
            println("The specified rftypes XML file '${input_XML_file_name}' is not a well-formed XML file")
            println (" ")
            println "Aborting .."
            /*throw new RuntimeException("The specified rftypes XML file '${input_XML_file_name}' is not a well-formed XML file", ex)*/
            System.exit(0)
        } 
        
        return input_XML_file_name
    }

    static def get_input_XML_file_name() {
    
        print "Please input a 'one-line' XML file : "
        def input_XML_file_name = System.in.newReader().readLine()
        
        def checked_input_XML_file_name = check_input_XML_file_name(input_XML_file_name)
        
        return checked_input_XML_file_name
    } 
    


    static void main(String... args) {
        
        println "\npretty print XML reformatter ${VersionString}\n\n"
        
        def my_xml_input_XML_file_name
     
        if ( args.size() == 0 ) {
          my_xml_input_XML_file_name = get_input_XML_file_name()
        }
        
        if ( args.size() == 1 ) {
          my_xml_input_XML_file_name = check_input_XML_file_name(args[0])
        }
        
        if ( args.size() > 1 ) {
          println "Too many command line parameters !"
          println "\nUsage: groovy make_pretty_print_XML.groovy [<xml_input_XML_file_name>]\n"
          println "Aborting .."
          System.exit(0)
        }
        
        println "The used input XML file is             : '${my_xml_input_XML_file_name}'"

        
        def xml = new XmlSlurper().parse(my_xml_input_XML_file_name)
        
 
        def String fileWithoutExt = my_xml_input_XML_file_name.take(my_xml_input_XML_file_name.lastIndexOf('.'))
        
        println "Pretty print XML output file           : '${fileWithoutExt}_PP.xml'\n\n"

        //Save File
        def writer = new FileWriter(fileWithoutExt+"_PP.xml")

        // Pretty print XML
        XmlUtil.serialize(xml, writer) 
  
      }
    }
