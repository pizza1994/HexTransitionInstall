#include "solver.h"
#include <cinolib/dual_mesh.h>
#include <cinolib/hex_transition_install.h>

inline bool polys_share_edge(const cinolib::AbstractPolyhedralMesh<> &m, const std::vector<uint> &polys)
{
    for(uint i = 0; i < polys.size(); i++)
    {
        for(uint j = 0; j < polys.size(); j++)
        {
            auto verts_h1 = m.poly_verts_id(polys[i]);
            auto verts_h2 = m.poly_verts_id(polys[j]);

            std::set<uint> s1(verts_h1.begin(), verts_h1.end());
            std::set<uint> s2(verts_h2.begin(), verts_h2.end());

            std::vector<uint> inters;
            std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(inters));

            if(inters.size() < 2)
                return false;
        }
    }
    return true;
}

void mark_candidates(cinolib::AbstractPolyhedralMesh<> &hm){

    for(uint vid=0; vid<hm.num_verts(); vid++){

        std::vector<uint> refinements;
        std::set<uint> refs_set;

        for(uint pid : hm.adj_v2p(vid)){
            refs_set.insert(hm.poly_data(pid).label);
            refinements.push_back(hm.poly_data(pid).label);
        }

        if(refs_set.size() == 2 && hm.adj_v2p(vid).size()==8){ //INSIDE
            std::sort(refinements.begin(), refinements.end());
            if(refinements[0] == refinements[3] && refinements[4] == refinements[7]){

                int ref = *refs_set.begin();


                std::vector<uint> cluster1;
                std::vector<uint> cluster2;

                for(uint pid : hm.adj_v2p(vid)){
                    if(hm.poly_data(pid).label == ref){
                        cluster1.push_back(pid);
                    }
                    else{
                        cluster2.push_back(pid);
                    }
                }

                if(polys_share_edge(hm, cluster1) && polys_share_edge(hm, cluster2))
                    hm.vert_data(vid).label = 1;
            }
        }
        else if(refs_set.size() == 2 && hm.adj_v2p(vid).size()==4 && hm.vert_is_on_srf(vid)){ //SURFACE
            std::sort(refinements.begin(), refinements.end());
            if(refinements[0] == refinements[1] && refinements[2] == refinements[3]){
                int ref = *refs_set.begin();


                std::vector<uint> cluster1;
                std::vector<uint> cluster2;

                for(uint pid : hm.adj_v2p(vid)){
                    if(hm.poly_data(pid).label == ref){
                        cluster1.push_back(pid);
                    }
                    else{
                        cluster2.push_back(pid);
                    }
                }

                if(polys_share_edge(hm, cluster1) && polys_share_edge(hm, cluster2))
                    hm.vert_data(vid).label = 1;
            }
        }
        else if(refs_set.size() == 2 && hm.adj_v2p(vid).size()==2 && hm.vert_is_on_srf(vid)){ //EDGE

            hm.vert_data(vid).label = 1;

        }


    }
}

int main(int argc, char *argv[])
{
    std::string input_grid_name = "";
    std::string output_grid_name = "";

    if(argc > 2){
        input_grid_name = std::string(argv[1]);
        output_grid_name = std::string(argv[2]); 
    }
    else{
        std::cerr<<"not enaugh arguments"<<std::endl;
    }

    cinolib::Polyhedralmesh<> m(input_grid_name.c_str());
    mark_candidates(m);
    bool passed=true;
    cinolib::Polyhedralmesh<> out;

    for(uint i : {4,3,2,5}){
        div_factor = i;
        solve(m);

        std::vector<bool> transition_verts(m.num_verts());
        for(uint vid=0; vid<m.num_verts(); vid++){
            if(m.vert_data(vid).label == 2){
                transition_verts[vid] = true;
            }
        }

        cinolib::hex_transition_install(m, transition_verts, out);


        cinolib::AABB box(out.vector_verts(), 0.99);

        for(uint fid=0; fid<out.num_faces(); fid++){
            if(out.face_is_on_srf(fid)){

                auto centroid = out.face_centroid(fid);

                if(box.contains(centroid, true)){
                    std::cout<<centroid<<" "<<out.bbox().min<<" "<<out.bbox().max<<std::endl;
                    std::cerr<<"Something went wrong, retrying with different paramenters"<<std::endl;
                    passed = false;
                    break;
                }
                else passed = true;
            }
        }
        break;
    }

    if(passed) std::cout<<"Schemes installed successfully :)"<<std::endl;
    else{
        std::cerr<<"Could not install schemes on this grid"<<std::endl;
        return -1;
    }

    std::cout<<"Making dual mesh..."<<std::endl;
    //DUAL::::::::::::::::::::::::::::::::::::::::::::::::::
    out.edge_mark_sharp_creases();
    cinolib::Polyhedralmesh<> dual;
    cinolib::dual_mesh(out, dual, true);
    std::vector<std::vector<uint>> polys(dual.num_polys());
    for(uint pid=0; pid<dual.num_polys();pid++){
        polys[pid] = dual.poly_verts_id(pid);
    }
    cinolib::Hexmesh<> dual_full_hexa(dual.vector_verts(), polys);

    for(uint pid=0; pid<dual_full_hexa.num_polys(); pid++){
        auto poly_verts = dual_full_hexa.poly_verts(pid);

        if(cinolib::hex_volume(poly_verts[0],poly_verts[1],poly_verts[2],poly_verts[3],poly_verts[4],poly_verts[5],poly_verts[6],poly_verts[7]) < 0){
            dual_full_hexa.poly_flip_winding(pid);
            dual_full_hexa.poly_reorder_p2v(pid);
        }
    }

    dual_full_hexa.save(output_grid_name.c_str());


    return 0;
}
