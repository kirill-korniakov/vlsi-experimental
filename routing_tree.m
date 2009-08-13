% COPYRIGHT C 2004-2005 by Zhuo (Robert) Li and Weiping Shi
% Permission to use for non-commercial purposes is granted provided
% that proper acknowledgments are given.
% For information about commercial use, contact 
% Zhuo (Robert) Li (zhybear@gmail.com), 
% Weiping Shi (wshi@ee.tamu.edu).

% This program is to display the routing tree and buffer position
% computed by buffer_insert or buffer_insert_bn2 program 
clear all
clear global
filename=input('Input tree file name:','s');
if isempty(filename)
   disp('File name should be specified!');
   return;
end
fid=fopen(filename);
if fid==-1
     disp('No such file in the directory');
     return;
end
string=fscanf(fid,'%s',1);
n_sinks=0;
nodes=[];
edges=[];
while(~strcmp(string,''))
     
      %Get driver(source) info 
      if length(string)>3 & strcmp(string(1:4),'driv')
         driver_x=str2num(fscanf(fid,'%s',1));
         if isempty(driver_x)
            disp('wrong format');
            return;
         end
         driver_y=str2num(fscanf(fid,'%s',1));
         if isempty(driver_y)
            disp('wrong format');
            return;
         end
         driver_r=fscanf(fid,'%s',1);
         nodes=[nodes;0, driver_x, driver_y];
      end  
      %Get sink info
      if length(string)>3 & strcmp(string(1:4),'sink')
         node_index=str2num(fscanf(fid,'%s',1));
         if isempty(node_index)
            disp('wrong format');
            return;
         end
         node_x=str2num(fscanf(fid,'%s',1));
         if isempty(node_x)
            disp('wrong format');
            return;
         end
         node_y=str2num(fscanf(fid,'%s',1));
         if isempty(node_y)
            disp('wrong format');
            return;
         end         
         node_c=fscanf(fid,'%s',1);
         node_q=fscanf(fid,'%s',1);
         nodes=[nodes;node_index, node_x, node_y];
         n_sinks=n_sinks+1;
      end  
      %Get candidate node
      if length(string)>3 & strcmp(string(1:4),'cand')
         node_index=str2num(fscanf(fid,'%s',1));
         if isempty(node_index)
            disp('wrong format');
            return;
         end
         node_x=str2num(fscanf(fid,'%s',1));
         if isempty(node_x)
            disp('wrong format');
            return;
         end
         node_y=str2num(fscanf(fid,'%s',1));
         if isempty(node_y)
            disp('wrong format');
            return;
         end         
         nodes=[nodes;node_index, node_x, node_y];         
      end  
      %Get edge info
      if length(string)>3 & strcmp(string(1:4),'edge')
         edge_s=str2num(fscanf(fid,'%s',1));
         if isempty(edge_s)
            disp('wrong format');
            return;
         end
         edge_t=str2num(fscanf(fid,'%s',1));
         if isempty(edge_t)
            disp('wrong format');
            return;
         end         
         edges=[edges;edge_s, edge_t];
      end              
      string=fscanf(fid,'%s',1);
  
end
fclose(fid);
[n_nodes, i]=size(nodes);
[n_edges, i]=size(edges);
low=min(nodes(:, 2:3));
high=max(nodes(:, 2:3));

% Read buffer position
filename=input('Input buffer location file name:','s');
if isempty(filename)
   disp('File name should be specified!');
   return;
end
fid=fopen(filename);
if fid==-1
     disp('No such file in the directory');
     return;
end
string=fscanf(fid,'%s',1);
buffer_position=[];
while(~strcmp(string,''))
     
      %Get buffer info 
      if length(string)>4 & strcmp(string(1:5),'start')
         x=str2num(fscanf(fid,'%s',1));
         if isempty(x)
            disp('wrong format');
            return;
         end
         fscanf(fid,'%s',1);
         y=str2num(fscanf(fid,'%s',1));
         if isempty(y)
            disp('wrong format');
            return;
         end
         k=fscanf(fid,'%s',1);
         buffertype=str2num(fscanf(fid,'%s',1));
         buffer_position=[buffer_position;x, y, buffertype];
      end  
      string=fscanf(fid,'%s',1);  
end
fclose(fid);
[n_buffer,i]=size(buffer_position);
% plot line
figure

for (i=1:n_edges)
   index_1=find(nodes(:,1)==edges(i,1));
   index_2=find(nodes(:,1)==edges(i,2));
   if (nodes(index_1, 2)~=nodes(index_2, 2))
      line([nodes(index_1, 2);nodes(index_2, 2)], [nodes(index_1, 3); nodes(index_1, 3)] ); 
   end
   if (nodes(index_1, 3)~=nodes(index_2, 3))
      line([nodes(index_2, 2);nodes(index_2, 2)], [nodes(index_1, 3); nodes(index_2, 3)]); 
   end   
end
hold on
%Plot sink position
for i=1:n_sinks
   plot(nodes(i+1,2), nodes(i+1,3), 'd','MarkerSize', 5);
end

%Plot buffer position
buffertype=['r', 'c', 'm', 'k', 'y', 'b', 'g', 'w'];
bufferdirection=['^','>','v','<'];
ratio=10;
for i=1:n_buffer
   
   index_1=find(nodes(:,1)==buffer_position(i,1));
   index_2=find(nodes(:,1)==buffer_position(i,2));

   if (nodes(index_1, 3)~=nodes(index_2, 3))
      position_y=nodes(index_1,3)+(nodes(index_2,3)-nodes(index_1,3))/ratio;
      position_x=nodes(index_1,2);
      direction=2+sign(nodes(index_2,3)-nodes(index_1,3));
   end   

   if (nodes(index_1, 2)~=nodes(index_2, 2))
      position_x=nodes(index_1,2)+(nodes(index_2,2)-nodes(index_1,2))/ratio;
      position_y=nodes(index_1,3);
      direction=3+sign(nodes(index_2,2)-nodes(index_1,2));
   end

   plot(position_x, position_y, strcat(buffertype(buffer_position(i, 3)),bufferdirection(direction)));
end
sprintf('Number of buffers %d', n_buffer)
plot(nodes(1,2), nodes(1,3), 'gs');

