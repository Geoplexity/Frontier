/**
 * Title:        ExtensionFileFilter
 * Description:  File filter by extension for use with JFileChooser
 */
   package sketcher;

   import javax.swing.filechooser.*;
   import java.io.File;


   public class ExtensionFileFilter extends FileFilter
   {
      UniqueStringArray exts = new UniqueStringArray(2);
      String description;
   
      public ExtensionFileFilter(String desc)
      {
         description = desc;
      }
   
      public void addExtension(String ext)
      {
         exts.add(ext);
      }
   
      public void setDescription(String desc)
      {
         description = desc;
      }
   
      public String getDescription()
      {
         return description;
      }
   
      public boolean accept(File parm1)
      {
         if (parm1.isDirectory()) 
            return true;
      
         String parmExt = parm1.toString().substring(parm1.toString().indexOf('.')+1,parm1.toString().length());
      
         for (int i=0; i<exts.size(); i++)
            if (exts.get(i).equalsIgnoreCase(parmExt))
               return true;
      
         return false;
      }
   }