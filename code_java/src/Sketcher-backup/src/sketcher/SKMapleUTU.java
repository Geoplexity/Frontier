/**
 * Title:        SKMapleUTU
 * Description:  UTU for Maple
 * Notes:        Command-line format for Maple V version 5:
 *                  > cmaple < input.txt > output.txt
 *
 *                * output.txt - output file created by Maple
 *                * input.txt  - input text file.  This is a normal text file with Maple commands 1 per line.
 *                    Example input file:
 *                      ****** Beginning of file ********
 *                      solve({x1=0,y1=0,y2=0,d1^2=(x2-x1)^2+(y2-y1)^2},{x1,x2,y1,y2});
 *                      ********* End of file ***********
 */
   package sketcher;

   import java.lang.StringBuffer;
   import java.io.BufferedWriter;
   import java.io.FileWriter;
   import java.io.BufferedReader;
   import java.io.FileReader;
   import java.io.IOException;

   public class SKMapleUTU extends SKUTU
   {
      public static String MapleDir = new String("d:\\maplev~1\\bin.wnt\\");
   
      public SKMapleUTU()
      {
      }
   
      private int getOtherID(SKBaseConstraint con, SKBaseShape sh)
      {
         if (con.ShapeList.get(0).equals(sh))
            return con.ShapeList.get(1).ID;
         else
            return con.ShapeList.get(0).ID;
      }
   
   //Finds a set of points to fix and adds their equations
      private int fixPoints(SKShapeArray shapes, SKConstraintArray cons, StringBuffer eqns)
      {
         int h;
         SKBaseShape sh;
         SKBaseConstraint con;
         for (int i=0; i<shapes.size(); i++)
         {
            sh = shapes.get(i);
         
            if (sh instanceof SKLineShape)
            {
               eqns.append("x"+sh.ID+"a=0,y"+sh.ID+"a=0,x"+sh.ID+"b="+((SKLineShape)sh).length+",");
               return i;
            }
            else
            {
               for (h=0; h<sh.ConstraintList.size(); h++)
               {
                  con = sh.ConstraintList.get(h);
                  if (con instanceof SKDistanceConstraint)
                  {
                     eqns.append("x"+sh.ID+"a=0,y"+sh.ID+"a=0,x"+getOtherID(con,sh)+"a="+((SKDistanceConstraint)con).distance+",");
                     return i;
                  }
               }
            }
         }
      
         return 0;
      }
   
      public void solveSystem(SKMainFrame frameMain, SKShapeArray shapes, SKConstraintArray cons)
      {
      //Notes: + Var names are:  {x/y/z/etc}+ShapeID+( (char)(ShapeVarNum+64) )
      //          Example: x0a = 0th shapes's first x var...for line, pt1's x
      //                   x0b = 0th shapes's second x var...for line, pt2's x
      //
      //       + eqns and vars MUST end with a comma, so make a comma the last character when appending
      
         if (shapes.size()<2)
            return;
      
      //list of equations to solve
         StringBuffer eqns = new StringBuffer(/*"x0a=0,y0a=0,"*/);
      
      //list of variables to solve for
         UniqueStringArray vars = new UniqueStringArray( shapes.size()*2 );
      
         int ix = fixPoints(shapes,cons,eqns);
      
         int deltaX = shapes.get(ix).getShapeX(),
         deltaY = shapes.get(ix).getShapeY();
      
      //eqns.append("x1 = 0, y1 = 0, y2 = 0, d1 ^ 2 = (x2 - x1) ^2 + (y2 - y1) ^ 2,");
      //vars.add(new String[] {"x1","x2","y1","y2"});
      
      //Add special shape equations
         SKBaseShape sh1,sh2;
         String v1,v2,v3,v4;
         int i;
         for (i=0; i<shapes.size(); i++)
         {
            sh1 = shapes.get(i);
            if (sh1 instanceof SKLineShape)
            { //Add distance equation for the line's pts
               v1 = new String("x"+sh1.ID+"a");
               v2 = new String("x"+sh1.ID+"b");
               v3 = new String("y"+sh1.ID+"a");
               v4 = new String("y"+sh1.ID+"b");
            
               eqns.append("(("+v1+"-"+v2+")^2+("+v3+"-"+v4+")^2)^.5 = "+((SKLineShape)sh1).length+",");
               vars.add(new String[] {v1,v2,v3,v4});
            }
         }
      
      //Add Constraint equations
         SKBaseConstraint con;
         for (i=0; i<cons.size(); i++)
         {
            con = cons.get(i);
            sh1 = con.ShapeList.get(0);
            sh2 = con.ShapeList.get(1);
            if (con instanceof SKDistanceConstraint)
            {
            //Determine which pts we are working with
               v1 = new String("x"+sh1.ID+(char)(con.getConInfo(sh1,0)+97));
               v2 = new String("x"+sh2.ID+(char)(con.getConInfo(sh2,0)+97));
               v3 = new String("y"+sh1.ID+(char)(con.getConInfo(sh1,0)+97));
               v4 = new String("y"+sh2.ID+(char)(con.getConInfo(sh2,0)+97));
            
               eqns.append("(("+v1+"-"+v2+")^2+("+v3+"-"+v4+")^2)^.5 = "+((SKDistanceConstraint)con).distance+",");
               vars.add(new String[] {v1,v2,v3,v4});
            }
            else if (con instanceof SKParallelConstraint)
            { //Note: MUST be 2 lines
               v1 = new String("x"+sh1.ID);
               v2 = new String("x"+sh2.ID);
               v3 = new String("y"+sh1.ID);
               v4 = new String("y"+sh2.ID);
            
               eqns.append("("+v1+"a-"+v1+"b)/("+v3+"a-"+v3+"b) = ("+v2+"a-"+v2+"b)/("+v4+"a-"+v4+"b),");
               vars.add(new String[] {v1+"a",v1+"b",v2+"a",v2+"b",v3+"a",v3+"b",v4+"a",v4+"b"});
            }
            else if (con instanceof SKPerpConstraint)
            { //Note: MUST be 2 lines
               v1 = new String("x"+sh1.ID);
               v2 = new String("x"+sh2.ID);
               v3 = new String("y"+sh1.ID);
               v4 = new String("y"+sh2.ID);
            
               eqns.append("("+v1+"a-"+v1+"b)/("+v3+"a-"+v3+"b) = -("+v2+"a-"+v2+"b)/("+v4+"a-"+v4+"b),");
               vars.add(new String[] {v1+"a",v1+"b",v2+"a",v2+"b",v3+"a",v3+"b",v4+"a",v4+"b"});
            }
            else if (con instanceof SKIncidenceConstraint)
            {
               v1 = new String("x"+sh1.ID);
               v2 = new String("x"+sh2.ID);
               v3 = new String("y"+sh1.ID);
               v4 = new String("y"+sh2.ID);
            
             //eqns.append("("+v1+"a-"+v1+"b)/("+v3+"a-"+v3+"b) = -("+v2+"a-"+v2+"b)/("+v4+"a-"+v4+"b),");
             //vars.add(new String[] {v1+"a",v1+"b",v2+"a",v2+"b",v3+"a",v3+"b",v4+"a",v4+"b"});
            }
         
         
         }
      
      //Create list of variables to solve for
         StringBuffer varsStr = new StringBuffer();
         for (int g=0; g<vars.size(); g++)
            varsStr.append(vars.get(g)+",");
      
      //Create string to insert into input file
         String toSolve = new String( "solve({"+eqns.substring(0,eqns.length()-1)+"},{"+varsStr.substring(0,varsStr.length()-1)+"});" );
      
      //Write temp file
         BufferedWriter bw;
         FileWriter fw;
         try
         {
            fw = new FileWriter(MapleDir+"input.txt");
            bw = new BufferedWriter(fw);
         }
            catch(IOException ioe)
            {
               System.out.println(ioe.getMessage());
               return;
            }
      
         try
         {
            bw.write(toSolve,0,toSolve.length());
         }
            catch(IOException ioe)
            {
               System.out.println(ioe.getMessage());
            }
            finally
            {
               try
               {
                  bw.flush();
                  bw.close();
               
                  fw.flush();
                  fw.close();
               }
                  catch(IOException ioe){
                  }
            }
      
      
      //Execute Maple & wait until its done
         try
         {
            Process pc = Runtime.getRuntime().exec("command.com /c "+MapleDir+"cmaple.exe < "+MapleDir+"input.txt > "+MapleDir+"output.txt");
            pc.waitFor();
         }
            catch(Exception ee)
            {
               System.out.println(ee.getMessage());
            };
      
      //Open and read output.txt generated by Maple for solution
         StringBuffer soln = new StringBuffer();
         BufferedReader br;
         FileReader fr;
         try
         {
            fr = new FileReader(MapleDir+"output.txt");
            br = new BufferedReader(fr);
         
         //Skip first 7 lines
            br.readLine();
            br.readLine();
            br.readLine();
            br.readLine();
            br.readLine();
            br.readLine();
            br.readLine();
         
         //Find start of solution (starts with a "{")
            String ln;
            do
            {
               ln = br.readLine();
            } while (ln.charAt(0) != '{');
         
         //Get entire (1st) solution in a StringBuffer
            int idx;
            do
            {
               idx = ln.indexOf("}");
               if (idx>-1)
                  soln.append( ln.substring(0,idx-1).trim() );
               else
                  soln.append( ln.trim() );
            
               ln = br.readLine();
            } while (idx == -1);
         
         //Close output.txt file
            br.close();
            fr.close();
         }
            catch(IOException ioe)
            {
               System.out.println(ioe.getMessage());
               return;
            }
      
      //Parse solution (in soln)
         char vr,mod;
         int sID;
         int g=1,
         h=0;
      
         do
         {
         //Get the variable parts
            vr = soln.charAt(g++);
         
            h=g;
            while (soln.charAt(g) > 47 && soln.charAt(g) < 58)
               g++;
            sID = new Integer( soln.substring(h,g) ).intValue();
         
            mod = soln.charAt(g++);
         
            while (soln.charAt(g) != '=')
               g++;
         
            h=++g;
         
            while (g<soln.length() && soln.charAt(g) != ',')
               g++;
         
            shapes.findByID(sID).updateFromUTU( vr, mod, deltaX, deltaY, new Double( soln.substring(h,g).trim() ).doubleValue() );
         
         //skip spaces
            g++;
            while (g<soln.length() && soln.charAt(g)==' ')
               g++;
         
         } while ( g<soln.length() );
      }
   }