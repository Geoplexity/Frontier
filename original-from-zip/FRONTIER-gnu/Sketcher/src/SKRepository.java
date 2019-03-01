/**
 * Title:        SKRepository
 * Description:  Window to display the user's repository
 */
/*Copyright (C) June 22, 2001 Meera Sitharam

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
http://www.gnu.org/copyleft/gpl.html

You should have received a copy of the GNU General Public License
in the documentation index in the documentation
folder of the FRONTIER-gnu directory; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

   package sketcher;

   import javax.swing.*;
   import java.awt.*;
   import java.util.Enumeration;
   import java.util.Vector;
   import java.io.File;
   import java.io.FilenameFilter;
   import java.io.IOException;
   import java.io.FileOutputStream;
   import java.util.zip.ZipFile;
   import java.util.zip.ZipInputStream;
   import java.util.zip.ZipOutputStream;
   import java.util.zip.ZipEntry;
   import java.awt.event.*;
   import javax.swing.event.*;

   public class SKRepository extends JFrame implements FilenameFilter
   {
      public static String repositoryDir;
      public Vector zEntries = new Vector();
   
      public SKMainFrame frameMain;
      public JComboBox cmbRepository = new JComboBox();
      public JMenuBar jMenuBar1 = new JMenuBar();
      public JMenu mniUserObjects = new JMenu();
      public JMenuItem mniCreateLibrary = new JMenuItem();
      public JMenuItem mniAddSelection = new JMenuItem();
      public JMenuItem mniRefresh = new JMenuItem();
      public JList lstObjects = new JList();
      public JMenuItem mniImportObject = new JMenuItem();
   
   
      public SKRepository()
      {
         try
         {
            jbInit();
         
            setSize(200,300);
         }
            catch(Exception e)
            {
               e.printStackTrace();
            }
      }
   
      public SKRepository(SKMainFrame frame)
      {
         this();
         frameMain = frame;
         setLocation(frameMain.getX(),frameMain.getY());
         repositoryDir = frameMain.HomeDir+File.separator+"repository"+File.separator;
      
         populateLibraries();
      }
   
      private void jbInit() throws Exception
      {
         this.setJMenuBar(jMenuBar1);
         this.setResizable(false);
         this.setTitle("Repository");
         mniUserObjects.setText("User Objects");
         mniUserObjects.addMenuListener(
                                 new javax.swing.event.MenuListener()
                                 {
                                 
                                    public void menuCanceled(MenuEvent e)
                                    {
                                    }
                                 
                                    public void menuDeselected(MenuEvent e)
                                    {
                                    }
                                 
                                    public void menuSelected(MenuEvent e)
                                    {
                                       mniUserObjects_menuSelected(e);
                                    }
                                 });
         mniCreateLibrary.setActionCommand("Create New Object Library");
         mniCreateLibrary.setText("Create New Object Library");
         mniCreateLibrary.addActionListener(
                                 new java.awt.event.ActionListener()
                                 {
                                 
                                    public void actionPerformed(ActionEvent e)
                                    {
                                       mniCreateLibrary_actionPerformed(e);
                                    }
                                 });
         mniAddSelection.setActionCommand("Add Selection to Library");
         mniAddSelection.setText("Add Selected Shapes to Library");
         mniAddSelection.addActionListener(
                                 new java.awt.event.ActionListener()
                                 {
                                 
                                    public void actionPerformed(ActionEvent e)
                                    {
                                       mniAddSelection_actionPerformed(e);
                                    }
                                 });
         mniRefresh.setText("Refresh Library List");
         mniRefresh.addActionListener(
                                 new java.awt.event.ActionListener()
                                 {
                                 
                                    public void actionPerformed(ActionEvent e)
                                    {
                                       mniRefresh_actionPerformed(e);
                                    }
                                 });
         cmbRepository.addActionListener(
                                 new java.awt.event.ActionListener()
                                 {
                                 
                                    public void actionPerformed(ActionEvent e)
                                    {
                                       cmbRepository_actionPerformed(e);
                                    }
                                 });
         lstObjects.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
         lstObjects.addMouseListener(
                                 new java.awt.event.MouseAdapter()
                                 {
                                 
                                    public void mouseClicked(MouseEvent e)
                                    {
                                       lstObjects_mouseClicked(e);
                                    }
                                 });
         lstObjects.setModel( 
                               new AbstractListModel()
                               {
                                  public int getSize()
                                  {
                                     return zEntries.size();
                                  }
                                  public Object getElementAt(int index)
                                  {
                                     return zEntries.get(index);
                                  }
                               }
                            );
         mniImportObject.setText("Import Selected User Object");
         mniImportObject.addActionListener(
                                 new java.awt.event.ActionListener()
                                 {
                                 
                                    public void actionPerformed(ActionEvent e)
                                    {
                                       mniImportObject_actionPerformed(e);
                                    }
                                 });
         this.getContentPane().add(cmbRepository, BorderLayout.NORTH);
         this.getContentPane().add(lstObjects, BorderLayout.CENTER);
         jMenuBar1.add(mniUserObjects);
         mniUserObjects.add(mniCreateLibrary);
         mniUserObjects.addSeparator();
         mniUserObjects.add(mniAddSelection);
         mniUserObjects.add(mniImportObject);
         mniUserObjects.addSeparator();
         mniUserObjects.add(mniRefresh);
      }
   
   //Used for file filters
      public boolean accept(File dir, String name)
      {
         return name.endsWith(".srp");
      }
   
   //Loads user libraries available in the HomeDir+"\repository" directory
      public void populateLibraries()
      {
         File f = new File(repositoryDir);
      
      //Create this dir if it doesn't exist
         if (!f.exists()) f.mkdir();
      
      //Get list of repository library files in this dir
         File[] files = f.listFiles( this );
      
         for (int i=0; i<files.length; i++)
         {
            cmbRepository.addItem( files[i].getName() );
         	System.out.println("repo items in cmb "+i);
         }
      }
   
      public void loadLibrary(String fileName)
      {
         try
         {
            ZipFile zFile = new ZipFile(fileName);
         
            zEntries.clear();
         
            Enumeration enum = zFile.entries();
            while (enum.hasMoreElements())
            {
               zEntries.add( enum.nextElement() );
            }
         
            lstObjects.updateUI();
         }
            catch(Exception e)
            { }
      }
   
      public void createNewLibrary()
      {
         File f;
         JOptionPane opt = new JOptionPane();
         String name;
         boolean firstTime = true;
         do
         {
            if (!firstTime)  JOptionPane.showConfirmDialog(this,"There is already a library with that name, please choose another.","Dupelicate Name",JOptionPane.OK_OPTION,JOptionPane.ERROR_MESSAGE);
         
            name = opt.showInputDialog(this,"Enter name for new object library:","New User Object Library",JOptionPane.QUESTION_MESSAGE);
            f = new File(repositoryDir+name+".srp");
            firstTime = false;
         }
         while ( name!=null && f.exists() );
      
      //Create file
         try
         {
            f.createNewFile();
            FileOutputStream fStream = new FileOutputStream(f);
            ZipOutputStream zip = new ZipOutputStream(fStream);
         
            zip.putNextEntry( new ZipEntry("~Default") );
            zip.close();
            fStream.close();
         }
            catch(Exception e)
            { }
      }
   
      void addSelection()
      {
         try
         {
            ZipFile f = new ZipFile( repositoryDir+cmbRepository.getSelectedItem() );
         
            boolean firstTime = true;
            String name;
            JOptionPane opt = new JOptionPane();
            do
            {
               if (!firstTime)  JOptionPane.showConfirmDialog(this,"There is already an object with that name, please choose another.","Dupelicate Name",JOptionPane.OK_OPTION,JOptionPane.ERROR_MESSAGE);
            
               name = opt.showInputDialog(this,"Enter name for new object:","New User Object",JOptionPane.QUESTION_MESSAGE);
               firstTime = false;
            }
            while ( f.getEntry(name)!=null );
         
            frameMain.addUserObject( f.getName(), name );
         }
            catch(Exception ex)
            { }
      }
   
      void importSelectedObject()
      {
         try
         {
            ZipFile zFile = new ZipFile( repositoryDir+cmbRepository.getSelectedItem().toString() );
            System.out.println("repository "+zFile.getInputStream( zFile.getEntry(lstObjects.getSelectedValue().toString()) ));
            frameMain.readFromStream( zFile.getInputStream( zFile.getEntry(lstObjects.getSelectedValue().toString()) ), false);
            zFile.close();
         }
            catch(Exception ex)
            { }
      }
   
      void mniCreateLibrary_actionPerformed(ActionEvent e)
      {
         createNewLibrary();
      }
   
      void mniRefresh_actionPerformed(ActionEvent e)
      {
         populateLibraries();
      }
   
      void mniUserObjects_menuSelected(MenuEvent e)
      {
         mniAddSelection.setEnabled( frameMain.SelectedShapes.size()>1 && cmbRepository.getSelectedItem()!=null );
      }
   
      void mniAddSelection_actionPerformed(ActionEvent e)
      {
         addSelection();
      }
   
      void cmbRepository_actionPerformed(ActionEvent e)
      {
         loadLibrary( repositoryDir+cmbRepository.getSelectedItem().toString() );
      }
   
      void mniImportObject_actionPerformed(ActionEvent e)
      {
         importSelectedObject();
      }
   
      void lstObjects_mouseClicked(MouseEvent e)
      {
         if (e.getClickCount()==2)
         { //double-clicked
            importSelectedObject();
         }
      }
   }
