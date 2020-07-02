#include <iostream>
#include <vector>
#include <string>
#include <initializer_list>

#include "dryadutils.h"

namespace dryad
{
    constexpr int SHARP = 1;
    constexpr int FLAT  = -1;

    constexpr int MAJOR = 0;
    constexpr int MINOR = 1;
    constexpr int DIM   = 2;
    constexpr int AUG   = 3;

    class degree_node
    {
        const int _degree;
        const int _triad;
        const int _alteration;
        bool _prog_entry;
        bool _prog_exit;

        std::string _name;
        std::vector<degree_node*> _edges;

        void build_name()
        {
            switch (_degree)
            {
            case 1: _name = "i"; break;
            case 2: _name = "ii"; break;
            case 3: _name = "iii"; break;
            case 4: _name = "iv"; break;
            case 5: _name = "v"; break;
            case 6: _name = "vi"; break;
            case 7: _name = "vii"; break;
            default:
                CRASHLOG("Unsupported degree value : " << _degree);
            }

            if (_triad == MAJOR || _triad == AUG) uppercase(_name);

            if (_alteration != 0) _name += _alteration == FLAT ? "b" : "#";
        }

    public:

        const std::string& get_name() const { return _name; }
        void set_prog_entry(bool value = true) { _prog_entry = value; }
        void set_prog_exit(bool value = true) { _prog_exit = value; }
        bool is_prog_entry() const { return _prog_entry; }
        bool is_prog_exit() const { return _prog_exit; }

        degree_node(int number, int triad, int alteration = 0)
            : _degree(number)
            , _triad(triad)
            , _alteration(alteration)
            , _prog_entry(false)
            , _prog_exit(false)
        {
            if (_degree < 1 || _degree > 7)                 CRASHLOG("Unsupported degree value : " << _degree);
            if (_triad < MAJOR || _triad > AUG)             CRASHLOG("Unsupported triad value : " << _triad);
            if (_alteration < FLAT || _alteration > SHARP)  CRASHLOG("Unsupported alteration value : " << _degree);
        }

        void add_edge(std::initializer_list<degree_node*> nodes)
        {
            _edges.insert(_edges.end(), nodes);
        }
    };

    class mode_graph
    {
        protected:
        std::vector<degree_node> degrees;
    };

    class major_graph : public mode_graph
    {
        std::vector<degree_node> degrees;
        std::vector<std::vector<degree_node*>> permutations;
        
        major_graph()
        {
            degrees.reserve(7);

            degrees.emplace_back(1, MAJOR);
            degrees.emplace_back(2, MINOR);
            degrees.emplace_back(3, MINOR);
            degrees.emplace_back(4, MAJOR);
            degrees.emplace_back(5, MAJOR);
            degrees.emplace_back(6, MINOR);
            degrees.emplace_back(7, DIM);

            degree_node* I    = &degrees[0];
            degree_node* ii   = &degrees[1];
            degree_node* iii  = &degrees[2];
            degree_node* IV   = &degrees[3];
            degree_node* V    = &degrees[4];
            degree_node* vi   = &degrees[5];
            degree_node* viid = &degrees[6];

            // Adding basic edges + circle of fifth
            //I    ->add_edge({ ii, iii, IV, V, vi, viid });
            //ii   ->add_edge({ I, IV, V, vi, viid });
            //iii  ->add_edge({ I, V, vi, viid });
            //IV   ->add_edge({ I, ii, V, vi, viid });
            //V    ->add_edge({ I, ii, IV });
            //vi   ->add_edge({ I, ii, iii, IV, V });
            //viid ->add_edge({ I, iii, IV });

            // Basic edges
            I    ->add_edge({ ii, iii, IV, V, vi, viid });
            ii   ->add_edge({ V, viid });
            iii  ->add_edge({ vi });
            IV   ->add_edge({ V, viid });
            V    ->add_edge({ I });
            vi   ->add_edge({ ii, IV });
            viid ->add_edge({ I, iii });
        }

        void generate_permutations()
        {
            permutations.reserve(0);




        }

    public:
        void print_permutations()
        {
            LOG(permutations.size() << " major permutations");

            for (std::vector<degree_node*>& progression : permutations)
            {
                int prog_counter = 0;

                std::cout << ++prog_counter << " : ";
                for (degree_node* degree : progression)
                {
                    std::cout << degree->get_name().c_str() << " ";
                } 
                std::cout << "\n";
            }
        }

    };
}