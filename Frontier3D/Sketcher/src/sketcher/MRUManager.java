/**
 * Title:        MRU Manager
 * Description:  Manages an MRU list
 */
   package sketcher;

   import java.awt.event.*;
   import javax.swing.ImageIcon;
   import java.io.File;
   import java.io.OutputStream;
   import java.io.InputStream;
   import java.io.FileOutputStream;
   import java.io.DataOutputStream;
   import java.io.IOException;
   import java.io.FileNotFoundException;
   import java.io.StreamCorruptedException;
   import java.io.FileInputStream;
   import java.io.DataInputStream;
   import javax.swing.JMenu;
   import javax.swing.JPopupMenu;
   import javax.swing.JMenuItem;

   public class MRUManager
   {
      MRUItemArray items;
      int          maxMRUItems;
      SKMainFrame  frameMain;
   
      public MRUManager(int maxItems, SKMainFrame MainFrame)
      {
         items = new MRUItemArray(maxItems);
         maxMRUItems = maxItems;
         frameMain = MainFrame;
      }
   
      public void addMRUItem(String display, String data)
      {
         items.add( new MRUItem(display,data) );
      }
   
      public void saveMRUtoFile(String fileName)
      {
         try
         {
            FileOutputStream fStream = new FileOutputStream(fileName);
            DataOutputStream p = new DataOutputStream(fStream);
         
            p.writeInt( items.size() );
         
            for (int i=0; i<items.size(); i++)
            {
               p.writeUTF( items.get(i).displayText );
               p.writeUTF( items.get(i).data );
            }
         
            p.flush();
            p.close();
            fStream.flush();
            fStream.close();
         }
            catch(Exception ee)
            { }
      }
   
      public void readMRUfromFile(String fileName)
      {
         try
         {
            FileInputStream fStream = new FileInputStream(fileName);
            DataInputStream p = new DataInputStream(fStream);
         
            int numToRead = p.readInt();
            if (numToRead > maxMRUItems)  numToRead = maxMRUItems;
         
            MRUItem newItem;
            String name, data;
            int cnt = 0;
            while (cnt < numToRead)
            {
               name = p.readUTF();
               data = p.readUTF();
            
               if (new File(data).exists())
                  items.add( new MRUItem( name, data ) );
            }
         
            p.close();
            fStream.close();
         }
            catch(Exception ee)
            { }
      }
   
      private void doClicked(MRUItem item)
      {
         frameMain.MRUclicked(item);
      }
   
      public void syncMenu(JMenu menu)
      {
         menu.removeAll();
      
         MRUMenuItem newItem;
         for (int i=0; i<items.size(); i++)
         {
            newItem = new MRUMenuItem(this,items.get(i));
            newItem.setIcon(new ImageIcon(frameMain.HomeDir+"/images/mruitem.gif"));
            newItem.addActionListener(
                                    new java.awt.event.ActionListener()
                                    {
                                       public void actionPerformed(ActionEvent e)
                                       {
                                          doClicked( ((MRUMenuItem)e.getSource()).MRUitem );
                                       }
                                    });
         
            menu.add(newItem);
         }
      }
   
      public void syncPopup(JPopupMenu popup)
      {
         popup.removeAll();
      
         MRUMenuItem newItem;
         for (int i=0; i<items.size(); i++)
         {
            newItem = new MRUMenuItem(this,items.get(i));
            newItem.setIcon(new ImageIcon(frameMain.HomeDir+"/images/mruitem.gif"));
            newItem.addActionListener(
                                    new java.awt.event.ActionListener()
                                    {
                                       public void actionPerformed(ActionEvent e)
                                       {
                                          doClicked( ((MRUMenuItem)e.getSource()).MRUitem );
                                       }
                                    });
         
            popup.add(newItem);
         }
      }
   }
