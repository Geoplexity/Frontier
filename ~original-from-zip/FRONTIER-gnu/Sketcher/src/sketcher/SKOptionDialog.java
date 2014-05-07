/**
 * Title:        SKOptionDialog
 * Description:  Allows user to modify options
 */
   package sketcher;

   import javax.swing.*;
   import javax.swing.event.*;
   import javax.swing.colorchooser.*;
   import java.awt.*;
   import java.awt.event.*;
   import java.beans.*;

   public class SKOptionDialog extends JDialog
   {
      private String  homeDir;
      private int     lastSelIdx = -1;
      public  boolean needRedoAllSolving = false;
   
      private JTabbedPane tabpaneMain = new JTabbedPane();
      private JPanel panelButtons = new JPanel();
      private JPanel panelGeneral = new JPanel();
      private JButton btnClose = new JButton();
   
      public JPanel panelColors = new JPanel();
      public JList lstColors = new JList();
      public BorderLayout borderLayout2 = new BorderLayout();
      public JColorChooser jColorChooser1 = new JColorChooser();
      public JLabel jLabel1 = new JLabel();
      public JLabel jLabel2 = new JLabel();
      public JLabel jLabel3 = new JLabel();
      public JLabel jLabel4 = new JLabel();
      public JLabel jLabel5 = new JLabel();
      public JComboBox cmbGroupDisplayMode = new JComboBox();
      public JComboBox cmbUnselGroupWhen = new JComboBox();
      public JComboBox cmbSimpleSolverMode = new JComboBox();
      public JComboBox cmbChooseSimpleSolver = new JComboBox();
      public JComboBox cmbChooseOnline = new JComboBox();
      public FlowLayout flowLayout1 = new FlowLayout();
   
      public SKOptionDialog(Frame owner, String title, boolean modal, String HomeDir)
      {
         super(owner,title,modal);
         homeDir = HomeDir;
      
         try
         {
            jbInit();
         
            setSize(600,350);
         
         //Center on desktop
            Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
            Dimension d = getSize();
            int x = (screen.width - d.width) >> 1;
            int y = (screen.height - d.height) >> 1;
            setLocation(x, y);
         }
            catch(Exception e)
            {
               e.printStackTrace();
            }
      }
   
      private void jbInit() throws Exception
      {
      //** MUST occur before cmb's eventhandlers are assigned
         cmbGroupDisplayMode.addItem("Show Group IDs");
         cmbGroupDisplayMode.addItem("Show Group IDs and Group Boxes (hides ungrouped shapes)");
         cmbUnselGroupWhen.addItem("When Unselecting Groups Only");
         cmbUnselGroupWhen.addItem("When Leaving Groups Tab");
         cmbSimpleSolverMode.addItem("Disable SimpleSolver");
         cmbSimpleSolverMode.addItem("Solve on assignment and AFTER dragging");
         cmbSimpleSolverMode.addItem("Solve on assignment and DURING dragging");
         cmbChooseSimpleSolver.addItem("Simple solver version 1.0");
         cmbChooseSimpleSolver.addItem("Simple solver version 2.0");
         cmbChooseOnline.addItem("Disable online solving");
         cmbChooseOnline.addItem("Online solving with bifurcations");
         cmbChooseOnline.addItem("Online soving with autosolve");
         btnClose.setMinimumSize(new Dimension(73, 27));
         btnClose.setPreferredSize(new Dimension(73, 27));
         btnClose.setText("Close");
         btnClose.addActionListener(
                                 new java.awt.event.ActionListener()
                                 {
                                 
                                    public void actionPerformed(ActionEvent e)
                                    {
                                       btnClose_actionPerformed(e);
                                    }
                                 });
         panelGeneral.setLayout(null);
         panelColors.setLayout(borderLayout2);
         lstColors.setMinimumSize(new Dimension(160, 346));
         lstColors.setPreferredSize(new Dimension(160, 346));
         lstColors.addListSelectionListener(
                                 new javax.swing.event.ListSelectionListener()
                                 {
                                 
                                    public void valueChanged(ListSelectionEvent e)
                                    {
                                       lstColors_valueChanged(e);
                                    }
                                 });
         jLabel1.setText("Group Display Mode");
         jLabel1.setBounds(new Rectangle(9, 7, 116, 17));
         cmbGroupDisplayMode.setBounds(new Rectangle(134, 7, 440, 20));
         cmbGroupDisplayMode.addItemListener(
                                 new java.awt.event.ItemListener()
                                 {
                                 
                                    public void itemStateChanged(ItemEvent e)
                                    {
                                       cmbGroupDisplayMode_itemStateChanged(e);
                                    }
                                 });
         jLabel2.setBounds(new Rectangle(9, 31, 116, 17));
         jLabel2.setText("Stop Group Display");
         cmbUnselGroupWhen.setBounds(new Rectangle(134, 31, 440, 20));
         cmbUnselGroupWhen.addItemListener(
                                 new java.awt.event.ItemListener()
                                 {
                                 
                                    public void itemStateChanged(ItemEvent e)
                                    {
                                       cmbUnselGroupWhen_itemStateChanged(e);
                                    }
                                 });
         this.addWindowListener(
                                 new java.awt.event.WindowAdapter()
                                 {
                                 
                                    public void windowOpened(WindowEvent e)
                                    {
                                       this_windowOpened(e);
                                    }
                                 });
         panelButtons.setLayout(flowLayout1);
         cmbSimpleSolverMode.addItemListener(
                                 new java.awt.event.ItemListener()
                                 {
                                 
                                    public void itemStateChanged(ItemEvent e)
                                    {
                                       cmbSimpleSolverMode_itemStateChanged(e);
                                    }
                                 });
         cmbSimpleSolverMode.setBounds(new Rectangle(134, 55, 440, 20));
         cmbChooseSimpleSolver.addItemListener(
                              
                                 new java.awt.event.ItemListener()
                                 
                                 {
                                 
                                 
                                    public void itemStateChanged(ItemEvent e)
                                    
                                    {
                                       cmbChooseSimpleSolver_itemStateChanged(e);
                                    }
                                 });
         cmbChooseSimpleSolver.setBounds(new Rectangle(134, 79, 440, 20));
         cmbChooseOnline.addItemListener(
                              
                                 new java.awt.event.ItemListener()
                                 {
                                 
                                    public void itemStateChanged(ItemEvent e)
                                    
                                    
                                    {
                                       cmbChooseOnline_itemStateChanged(e);
                                    }
                                 });
         cmbChooseOnline.setBounds(new Rectangle(134, 103, 440, 20));
      
         jLabel3.setText("SimpleSolver Mode");
         jLabel3.setBounds(new Rectangle(9, 55, 116, 17));
         jLabel4.setText("Choose SimpleSolver version");
         jLabel4.setBounds(new Rectangle(9, 79, 116, 17));
         jLabel5.setText("Choose online/offline");
         jLabel5.setBounds(new Rectangle(9, 103, 116, 17));
      
         this.addWindowListener(
                                 new java.awt.event.WindowAdapter()
                                 {
                                 
                                    public void windowClosed(WindowEvent e)
                                    {
                                       this_windowClosed(e);
                                    }
                                 });
         this.getContentPane().add(tabpaneMain, BorderLayout.CENTER);
         tabpaneMain.add(panelGeneral, "General");
         panelGeneral.add(cmbGroupDisplayMode, null);
         panelGeneral.add(cmbUnselGroupWhen, null);
         panelGeneral.add(cmbSimpleSolverMode, null);
         panelGeneral.add(cmbChooseSimpleSolver, null);
         panelGeneral.add(cmbChooseOnline, null);
         panelGeneral.add(jLabel1, null);
         panelGeneral.add(jLabel2, null);
         panelGeneral.add(jLabel3, null);
         panelGeneral.add(jLabel4, null);
         panelGeneral.add(jLabel5, null);
         tabpaneMain.add(panelColors, "Colors");
         panelColors.add(lstColors, BorderLayout.WEST);
         panelColors.add(jColorChooser1, BorderLayout.CENTER);
         this.getContentPane().add(panelButtons, BorderLayout.SOUTH);
         panelButtons.add(btnClose, null);
      
         lstColors.setModel( 
                              new ListModel()
                              {
                                 public Object getElementAt(int index)
                                 {
                                    return SKOptions.colorOptionNames[index];
                                 }
                                 public int getSize()
                                 {
                                    return SKOptions.colorOptions.length;
                                 }
                                 public void addListDataListener(ListDataListener l)
                                 {}
                                 public void removeListDataListener(ListDataListener l)
                                 {}
                              } );
      }
   
      void this_windowOpened(WindowEvent e)
      {
         cmbGroupDisplayMode.setSelectedIndex( SKOptions.byteOptions[ SKOptions.groupDisplayMode ] );
         cmbUnselGroupWhen.setSelectedIndex( SKOptions.byteOptions[ SKOptions.unSelGroupsWhen ] );
         cmbSimpleSolverMode.setSelectedIndex( SKOptions.byteOptions[ SKOptions.simpleSolverMode ] );
         cmbChooseSimpleSolver.setSelectedIndex( SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] );
         cmbChooseOnline.setSelectedIndex( SKOptions.byteOptions[ SKOptions.onlineMode ] );
      }
   
      void this_windowClosed(WindowEvent e)
      {
         if (lastSelIdx > -1)
            SKOptions.colorOptions[lastSelIdx] = jColorChooser1.getColor();
      
         SKOptions.saveOptions(homeDir+SKOptions.optionFileName);
      }
      void btnClose_actionPerformed(ActionEvent e)
      {
         hide();
      }
   
      void lstColors_valueChanged(ListSelectionEvent e)
      {
         if (lastSelIdx > -1)
            SKOptions.colorOptions[lastSelIdx] = jColorChooser1.getColor();
      
         lastSelIdx = lstColors.getSelectedIndex();
         if (lastSelIdx > -1)
            jColorChooser1.setColor( SKOptions.colorOptions[lastSelIdx] );
      }
   
      void cmbGroupDisplayMode_itemStateChanged(ItemEvent e)
      {
         SKOptions.byteOptions[ SKOptions.groupDisplayMode ] = (byte)cmbGroupDisplayMode.getSelectedIndex();
      }
   
      void cmbUnselGroupWhen_itemStateChanged(ItemEvent e)
      {
         SKOptions.byteOptions[ SKOptions.unSelGroupsWhen ] = (byte)cmbUnselGroupWhen.getSelectedIndex();
      }
   
      void cmbSimpleSolverMode_itemStateChanged(ItemEvent e)
      {
         if (e.getStateChange()==e.SELECTED)
         {
            needRedoAllSolving = SKOptions.byteOptions[ SKOptions.simpleSolverMode ]==0;
            SKOptions.byteOptions[ SKOptions.simpleSolverMode ] = (byte)cmbSimpleSolverMode.getSelectedIndex();
            if (needRedoAllSolving)  needRedoAllSolving = SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0;
         }
      }
      void cmbChooseSimpleSolver_itemStateChanged(ItemEvent e)
      {
         if (e.getStateChange()==e.SELECTED)
            SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] = (byte)cmbChooseSimpleSolver.getSelectedIndex();
      }
      void cmbChooseOnline_itemStateChanged(ItemEvent e)
      {
         if (e.getStateChange()==e.SELECTED)
            SKOptions.byteOptions[ SKOptions.onlineMode ] = (byte)cmbChooseOnline.getSelectedIndex();
      }
   }