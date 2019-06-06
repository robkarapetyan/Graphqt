#include "graph.h"
#include <QDebug>

Graph::Graph(){
   // nodes.push_back(nullptr);
}
inline void Graph::add_vertex(QPoint coord)
{
    Node * t = new Node;
    t->m_point = coord;
    nodes.push_back(t);
}

void Graph::add_edge(Node *from, Node *to)
{
    if(from->m_nexts.empty())
    {
        from->m_nexts.push_back(to);
    }
    else{
    auto ite = std::find(from->m_nexts.begin(), from->m_nexts.end(), to);
    if (ite == from->m_nexts.end())
    {
        from->m_nexts.push_back(to);
    }
    }
    if(to->m_nexts.empty())
    {
       to->m_nexts.push_back(from);
    }
    else{

    auto ite1 = std::find(to->m_nexts.begin(), to->m_nexts.end(), from);
    if (ite1 == to->m_nexts.end())
    {
        to->m_nexts.push_back(from);
    }
    }
}


//---------------------------------------DFS's-------------------------
inline void Graph::DFS(Node* a)
{
    visited_vertexes.clear();
    just_DFS(a);
}

inline void Graph::just_DFS(Node * a)
{
    visited_vertexes.push_back(a);
    for (auto it = a->m_nexts.begin(); it != a->m_nexts.end(); ++it) {
        auto ite = std::find(visited_vertexes.begin(), visited_vertexes.end(), *it);
        if (ite == visited_vertexes.end()) {
            just_DFS(*it);
        }
    }
}

inline void Graph::DFS_with_ignore(Node* start_vertex, Node* ignoring_vertex)
{
    visited_vertexes.clear();
    DFS_w_i(start_vertex, ignoring_vertex);
}

inline void Graph::DFS_w_i(Node* start_vertex, Node* ignoring_vertex)
{
    visited_vertexes.push_back(start_vertex);
    for (auto it = start_vertex->m_nexts.begin(); it != start_vertex->m_nexts.end(); ++it) {
        auto ite = std::find(visited_vertexes.begin(), visited_vertexes.end(), *it);
        if (ite == visited_vertexes.end() && *it != ignoring_vertex) {
            DFS_w_i(*it, ignoring_vertex);
        }
    }
}

inline void Graph::DFS_for_blocks(Node* start_vertex,Node*ignoring_vertex)
{
     auto itk = find(visited_vertexes.begin(), visited_vertexes.end(), start_vertex);
     if (itk == visited_vertexes.end()) {
         visited_vertexes.push_back(start_vertex);
         current_block.push_back(start_vertex);
         for (auto it = start_vertex->m_nexts.begin(); it != start_vertex->m_nexts.end(); ++it) {
             auto ite = find(current_block.begin(), current_block.end(), *it);

             if (ite == current_block.end() && *it != ignoring_vertex) {
                 if (!(is_cutpoint(*it))) {
                     DFS_for_blocks(*it, ignoring_vertex);
                 }
                 else {
                     current_block.push_back(*it);
                 }
             }
         }
     }
}

void Graph::DFS_for_circle_search(Node *a)
{
    visited_vertexes.push_back(a);
    for (auto it = a->m_nexts.begin(); it != a->m_nexts.end(); ++it) {
        if( is_cutpoint(*it))
        {
            auto ite = std::find(visited_vertexes.begin(), visited_vertexes.end(), *it);
            if (ite == visited_vertexes.end() )
            {
                if(has_more_than1(*it))
                {
                DFS_for_circle_search(*it);
                }
            }else if(*it == visited_vertexes[0])
            {
              break;
            }
        }
    }
}


int Graph::circle_search_dfs(Node * a)
{
   visited_vertexes.clear();
   DFS_for_circle_search(a);
   if(visited_vertexes.size() == 1)
   {
       return 0;
   }
   else if(visited_vertexes.size() == 2)
   {
       return 2;
   }
   else
   {
       return 3;
   }
}

//---------------------------------cutpoints----------------------------

inline void Graph::get_cutpoints()
{
    if (cutpoints.empty())
            {
                //calc_cutpoints();
            }
    for (size_t i = 0; i < cutpoints.size(); ++i) {
        for (size_t k = 0; k < nodes.size(); ++k)
        {
            if (cutpoints[i] == nodes[k])
            {
                qDebug() << k;
            }
        }
    }
    qDebug() << "";
}

void Graph::calc_cutpoints()
{
    cutpoints.clear();
    for (size_t i = 0; i < nodes.size(); ++i) {
            DFS(nodes[i]);
            std::vector<Node*> tmp = visited_vertexes;
            DFS_with_ignore(nodes[i]->m_nexts[0], nodes[i]);
            if (tmp.size() - visited_vertexes.size() > 1) {
                cutpoints.push_back(nodes[i]);
        }
    }
}

void Graph::calc_cutpoints(Node* forx)
{
        DFS(forx);
        std::vector<Node*> tmp = visited_vertexes;
        DFS_with_ignore(forx->m_nexts[0], forx);
        if (tmp.size() - visited_vertexes.size() > 1)
        {
            cutpoints.push_back(forx);
        }
}

inline bool Graph::is_cutpoint(Node* a)
{
    for (size_t i = 0; i < cutpoints.size(); ++i) {
        if (a == cutpoints[i])
        {
            return true;
        }
    }
    return false;
}

//---------------------------------blocks-&-bridges----------------------

void Graph::calc_blocks()
{
    std::vector<Node*> nexts_of_current_cutpoint_wich_are_cutpoints_too;
    if(!is_connected())
    {
        seperate_to_subgraphs();
        blocks.clear();
        bridges.clear();
        cutpoints.clear();
        for (size_t i = 0; i < subgraphs.size(); ++i)
        {
            for (size_t j = 0; j < subgraphs[i].size(); ++j)
            {
                calc_cutpoints(subgraphs[i][j]);
            }
        }
        qDebug() << "";
        qDebug() << cutpoints.size();
        qDebug() << "";

        visited_vertexes.clear();
        for (size_t i = 0; i < cutpoints.size(); ++i) {
            nexts_of_current_cutpoint_wich_are_cutpoints_too.clear();
            for (size_t j = 0; j < cutpoints[i]->m_nexts.size(); ++j) {
                if (!is_cutpoint(cutpoints[i]->m_nexts[j])) { //The next vertex isn't cutpoint
                    current_block.clear();
                    DFS_for_blocks(cutpoints[i]->m_nexts[j], cutpoints[i]);
                    auto it1 = std::find(current_block.begin(), current_block.end(), cutpoints[i]);
                    if (!current_block.empty()) {
                        if (it1 == current_block.end()) { current_block.push_back(cutpoints[i]); }
                        blocks.push_back(current_block);
                    }
                }
                else {
                    bool x = false;
                     for (size_t t = 0; t < cutpoints[i]->m_nexts.size(); ++t)
                     {
                         if(cutpoints[i]->m_nexts[t] != cutpoints[i]->m_nexts[i] )
                         {
                    DFS_with_ignore(cutpoints[i]->m_nexts[t], cutpoints[i]);
                     auto it1 = std::find(visited_vertexes.begin(), visited_vertexes.end(), cutpoints[i]->m_nexts[j]);
                     if(it1 != visited_vertexes.end()){x = true;}

                     }

                     qDebug() << "";
                     qDebug() << x;
                     qDebug() << i;
                     qDebug() << "";

                     if(x == true)
                     {
                         if(circle_search_dfs((cutpoints[i])))
                         {
                             std::sort(visited_vertexes.begin(),visited_vertexes.end());
                             bool y = false;
                             for (size_t n = 0; n < blocks.size(); ++n)
                             {
                                 if(blocks[i].size() == visited_vertexes.size() && blocks[i][0] == visited_vertexes[0])
                                 {
                                     y = true;
                                 }
                             }
                             if(y == false)
                             {
                             blocks.push_back(visited_vertexes);
                             }
                         }
                     }
                     else{

                         std::vector<Node*> tp;
                         tp.push_back(cutpoints[i]->m_nexts[j]);
                         tp.push_back(cutpoints[i]);

                         bridges.push_back(tp);
                         blocks.push_back(tp);



                     }
                     }


                }
            }
        }


    }
    else
    {

    calc_cutpoints();
    blocks.clear();
    bridges.clear();
    visited_vertexes.clear();
    for (size_t i = 0; i < cutpoints.size(); ++i) {
        nexts_of_current_cutpoint_wich_are_cutpoints_too.clear();
        for (size_t j = 0; j < cutpoints[i]->m_nexts.size(); ++j) {
            if (!is_cutpoint(cutpoints[i]->m_nexts[j])) { //The next vertex isn't cutpoint
                current_block.clear();
                DFS_for_blocks(cutpoints[i]->m_nexts[j], cutpoints[i]);
                auto it1 = std::find(current_block.begin(), current_block.end(), cutpoints[i]);
                if (!current_block.empty()) {
                    if (it1 == current_block.end()) { current_block.push_back(cutpoints[i]); }
                    blocks.push_back(current_block);
                }
            }
            else {
                nexts_of_current_cutpoint_wich_are_cutpoints_too.push_back(cutpoints[i]->m_nexts[j]);

            }
        }



            if(circle_search_dfs(cutpoints[i]))
            {
                qDebug() << "sasa";
                std::sort(visited_vertexes.begin(),visited_vertexes.end());
                auto it1 = std::find(blocks.begin(),blocks.end(),visited_vertexes);
                if(it1 == blocks.end())
                {
                     blocks.push_back(visited_vertexes);
                }

            }

             //qDebug() << circle_search_dfs(cutpoints[i]) ;




             for (size_t k = 0; k < nexts_of_current_cutpoint_wich_are_cutpoints_too.size(); ++k)
             {


        }
           /*  for (size_t j = 0; j < cutpoints[i]->m_nexts.size(); ++j)
             {
                  DFS_with_ignore(cutpoints[i]->m_nexts[j],cutpoints[i]);
                  auto it1 = std::find(visited_vertexes.begin(), visited_vertexes.end(), nexts_of_current_cutpoint_wich_are_cutpoints_too[k]);
                  if(it1 == visited_vertexes.end())
                  {
                      if(circle_search_dfs(cutpoints[i]))
                      blocks.push_back(visited_vertexes);
                  }



          if (!search_items_in_blocks(cutpoints[i],  nexts_of_current_cutpoint_wich_are_cutpoints_too[k]))
          {
              current_block.clear();
            //Denying repeat of the bridges
              bool a = false;
              if (!bridges.empty())
              {
                  for (size_t t = 0; t < bridges.size(); ++t) {
                      if (bridges[t][0] == cutpoints[i] && bridges[t][1] == nexts_of_current_cutpoint_wich_are_cutpoints_too[k])
                      {
                          a = true;
                      }
                  }
                  if (a == false)
                  {
                    if(nexts_of_current_cutpoint_wich_are_cutpoints_too[k] != cutpoints[i])
                    {
                        current_block.push_back(nexts_of_current_cutpoint_wich_are_cutpoints_too[k]);
                        current_block.push_back(cutpoints[i]);
                       // blocks.push_back(current_block);
                        bridges.push_back(current_block);
                    }
                  }
              }
            else {
                if(nexts_of_current_cutpoint_wich_are_cutpoints_too[k] != cutpoints[i])
                {
                    current_block.push_back(nexts_of_current_cutpoint_wich_are_cutpoints_too[k]);
                    current_block.push_back(cutpoints[i]);
                    //blocks.push_back(current_block);
                    bridges.push_back(current_block);
                }

             }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

        /*auto it1 = std::find(visited_vertexes.begin(), visited_vertexes.end(), cutpoints[i]);

       if(circle_search_dfs(cutpoints[i]))
       {
          for(size_t t = 0; t < bridges.size(); ++i)
          {
              for (size_t k = 0; k < visited_vertexes.size() - 1; ++k)
              {
                  if((bridges[t][0] == visited_vertexes[k] && bridges[t][1] == visited_vertexes[k + 1] ) || (bridges[t][1] == visited_vertexes[k] && bridges[t][0] == visited_vertexes[k + 1] ))
                  {
                      bridges.erase(bridges.begin() + t);
                  }
              }
          }
       }
       if(it1 == visited_vertexes.end())
       {
           blocks.push_back(visited_vertexes);
       }

*/
    }
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        if(nodes[i]->m_nexts.size() == 1 && is_cutpoint(nodes[i]->m_nexts[0]))
        {
            std::vector<Node*> tmp1;
            tmp1.push_back(nodes[i]);
            tmp1.push_back(nodes[i]->m_nexts[0]);
            bridges.push_back(tmp1);
            //blocks.push_back(tmp1);
        }
    }


    }
}

bool Graph::search_items_in_blocks(Node* a, Node* b) {
    bool first = true;
    bool second = false;
    if (!blocks.empty()) {
        for (size_t i = 0; i < blocks.size(); ++i) {
            for (size_t j = 0; j < blocks[i].size(); ++j) {
                if (blocks[i][j] == a) {
                    first = true;
                }
                else if (blocks[i][j] == b) {
                    second = true;
                }
            }
        }
        if (first && second) {
            return true;
        }
    }
    return false;
}

bool Graph::is_connected()
{
    DFS(nodes[0]);
    if(visited_vertexes.size() < nodes.size())
    {
        return false;
    }
    return true;
}

void Graph::seperate_to_subgraphs()
{
    if(!is_connected())
    {
    visited_vertexes.clear();
    subgraphs.clear();
    for (size_t i = 0; i < nodes.size(); ++i) {

        if(!visited_vertexes.empty())
        {
            bool k = false;
            for (size_t j = 0; j < visited_vertexes.size(); ++j)
            {
                if(nodes[i] == visited_vertexes[j])
                {
                    k = true;
                }
            }
            if(k == false)
            {
                DFS(nodes[i]);
                subgraphs.push_back(visited_vertexes);
            }
        }
        else {
            DFS(nodes[i]);
            subgraphs.push_back(visited_vertexes);
        }
    }
    }
}

void Graph::add_edge_to_node_slot(Node* from,Node* to)
{
    add_edge(from,to);
}

void Graph::show_blocks_slot()
{
    calc_blocks();
    if(index < blocks.size())
    {
        for (size_t k = 0; k < nodes.size(); ++k)
        {
            nodes[k]->a.setRgb(250,250,0,250);
        }
        for (size_t j = 0; j < blocks[index].size(); ++j)
        {
            blocks[index][j]->a.setRgb(100,10,100,250);
        }
        ++index;
    }else{
        index = 0;
        for (size_t k = 0; k < nodes.size(); ++k)
        {
            nodes[k]->a.setRgb(250,250,0,250);
        }
        for (size_t j = 0; j < blocks[index].size(); ++j)
        {
        blocks[index][j]->a.setRgb(100,10,100,250);
        }
        ++index;
    }
}

void Graph::show_bridges_slot()
{
    /*seperate_to_subgraphs();

    for (size_t i = 0; i < subgraphs.size(); ++i)
    {
        for (size_t j = 0;j < subgraphs[i].size(); ++j)
        {
            for (size_t k = 0;k < nodes.size(); ++k)
            {
            if(subgraphs[i][j] == nodes[k])
            {
                qDebug() << k;
            }
            }
        }
        qDebug() << "k";
    }
    //qDebug() << subgraphs.size();
    //bridges.clear();*/

    calc_blocks();

    if(ind1 < bridges.size())
    {
        for (size_t k = 0; k < nodes.size(); ++k)
        {
            nodes[k]->a.setRgb(250,250,0,250);
        }
        for (size_t j = 0; j < bridges[ind1].size(); ++j)
        {
            bridges[ind1][j]->a.setRgb(100,100,10,250);
        }
        ++ind1;
    }else{
        ind1 = 0;
        for (size_t k = 0; k < nodes.size(); ++k)
        {
            nodes[k]->a.setRgb(250,250,0,250);
        }
        for (size_t j = 0; j < bridges[ind1].size(); ++j)
        {
        bridges[ind1][j]->a.setRgb(100,100,10,250);
        }
        ++ind1;
    }


}


bool Graph::has_more_than1(Node* a){
    int count = 0;
     for (size_t j = 0; j < a->m_nexts.size(); ++j) {
         if(is_cutpoint(a->m_nexts[j]))
         {
             count++;
         }
     }
     if(count > 1){
         return true;
     }
     else {
         return false;
     }
}



