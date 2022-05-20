#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace std;
using namespace sf;

//������ � ������� ���� ������
const int R = 50;
const int D = R * 2;

//���������� ������ ����� �� �������� 11 ������������ ������
const int N = 7;

//�����
const string FONT = "Arial.ttf";

//������� ������
const int btn_width = 115;
const int btn_height = 25;

//����� ��������, ����������� � ������
ostream& operator<<(ostream& stream, const vector<string>& to_print)
{
	if (to_print.size() > 0)
	{
		for (int i = 0; i < to_print.size(); ++i)
		{
			stream << to_print[i];
			if (i != to_print.size() - 1)
			{
				stream << "-";
			}
		}
	}
	else
	{
		stream << "-";
	}
	
	stream << endl;
	return stream;
}

//����� �������
void print(const vector<vector<int>>& matrix, const vector<string> names)
{
	int size = names.size();

	for (int i = 0; i < size; ++i)
	{
		cout << i << ": " << names[i] << endl;
	}

	cout << endl << "-  ";

	for (int i = 0; i < size; ++i)
	{
		cout << i << "   ";
	}

	cout << endl;

	for (int i = 0; i < size; ++i)
	{
		cout << i << " ";

		for (int j = 0; j < size; ++j)
		{
			if (matrix[i][j] >= 0 && matrix[i][j] <= 9)
			{
				cout << " " << matrix[i][j] << "  ";
			}
			else
			{
				cout << " " << matrix[i][j] << " ";
			}
		}

		cout << endl;
	}
}

//���������, ���� �� ������� � �������
bool check_insertion(const vector<string>& vector, const string& element)
{
	for (int i = 0; i < vector.size(); ++i)
	{
		if (vector[i] == element)
		{
			return true;
		}
	}
	return false;
}

struct Coords
{
	double x = 0;
	double y = 0;

	void set(const double& x, const double& y)
	{
		this->x = x;
		this->y = y;
	}
};

//��������� ������� �� �������� �����������. ���� ������� �� �������� - �������� �������, ����� - �������
void draw_vertex(const Coords& coords, const sf::String& txt, const bool& visited, RenderWindow& window)
{
	CircleShape circle(R);
	circle.setOrigin(R, R);
	circle.setPosition(coords.x, coords.y);
	circle.setOutlineColor(Color::Black);
	circle.setOutlineThickness(3);
	if (visited == true)
	{
		circle.setFillColor(Color::Green);
	}
	else
	{
		circle.setFillColor(Color::Red);
	}

	Font font;
	font.loadFromFile(FONT);

	Text text(txt, font, R / 3);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(coords.x, coords.y);
	text.setFillColor(Color::Black);

	window.draw(circle);
	window.draw(text);
}

//��������� ����� �� ����������� ���� ������, � ����� ��������� ���������� ����� ��������� ������
void draw_line(const Coords& coords1, const Coords& coords2, const sf::String& txt, RenderWindow& window)
{
	vector<Vertex> line =
	{
		Vertex(Vector2f(coords1.x, coords1.y), Color::Black),
		Vertex(Vector2f(coords2.x, coords2.y), Color::Black)
	};

	Font font;
	font.loadFromFile(FONT);

	Text text(txt, font, R / 3);
	text.setPosition((coords1.x + coords2.x) / 2, (coords1.y + coords2.y) / 2);
	text.setOutlineThickness(5);
	text.setOutlineColor(Color::White);
	text.setFillColor(Color::Black);

	window.draw(&line[0], line.size(), Lines);
	window.draw(text);
}

//��������� ������ �� ����������� � �� ������
void draw_button(const Coords& coords, const int& width, const int& height, const string& txt, RenderWindow& window)
{
	RectangleShape button(Vector2f(width, height));
	button.setOrigin(width / 2, height / 2);
	button.setPosition(coords.x, coords.y);
	button.setFillColor(Color::Red);
	button.setOutlineThickness(3);
	button.setOutlineColor(Color::Black);

	Font font;
	font.loadFromFile(FONT);

	Text text(txt, font, R / 3);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(coords.x, coords.y);
	text.setFillColor(Color::Black);

	window.draw(button);
	window.draw(text);
}

//��������� ��������� �� ����������� � ������
void draw_message(const Coords& coords, const string& txt, bool centered, RenderWindow& window)
{
	Font font;
	font.loadFromFile(FONT);

	Text message(txt, font, R / 3);
	if (centered == true)
	{
		message.setOrigin(message.getLocalBounds().width / 2, message.getLocalBounds().height / 2);
	}
	message.setPosition(coords.x, coords.y);
	message.setFillColor(Color::Black);

	window.draw(message);
}

class Graph
{
public:
	vector<string> vertex_list;
	vector<vector<int>> matrix;

	Graph()
	{

	}

	Graph(const Graph& another_one)
	{
		vertex_list = another_one.vertex_list;
		matrix = another_one.matrix;
	}

	Graph(const int& size)
	{
		matrix = vector<vector<int>>(size, vector<int>(size));

		for (int i = 0; i < size; ++i)
		{
			for (int j = 0; j < size; ++j)
			{
				matrix[i][j] = 0;
			}
		}
	}	

	int size()
	{
		return vertex_list.size();
	}	

	//���������� ������ ������� (��� -1, ���� ������� ��� � �����)
	int get_index(const string& vertex)
	{
		for (int i = 0; i < size(); ++i)
		{
			if (vertex_list[i] == vertex)
			{
				return i;
			}
		}
		return -1;
	}

	//���������� ����� ���� ����� ����� ��������� ��������� (��� 0, ���� ���� �� ����� �� ������ ��� � �����)
	int get_weight(const string& vertex1, const string& vertex2)
	{
		int pos1 = get_index(vertex1);
		int pos2 = get_index(vertex2);

		if (pos1 == -1 || pos2 == -1)
		{
			cout << "��� ������� ���� �� ������ ����������� � �����" << " " << vertex1 << " " << vertex2 << endl;
			return 0;
		}

		return matrix[pos1][pos2];
	}

	//���������� ������ � ����������� �� ������� �������� �������
	vector<string> get_neighbour_list(const string& vertex)
	{
		vector<string> neighbour_list;
		int pos = get_index(vertex);

		if (pos != -1)
		{
			for (int i = 0; i < size(); ++i)
			{
				if (matrix[pos][i] > 0)
				{
					neighbour_list.push_back(vertex_list[i]);
				}
			}
		}

		return neighbour_list;
	}

	//����������� ������� ������ ������������
	vector<string> recursive_sailsman(vector<string> visited, string finish)
	{
		//��� ��������� ����
		vector<vector<string>> options;

		//���������� ���� � ��� �����
		vector<string> final_route;
		int shortest = -1;

		//������������� ���� ��� ���������� ���������� �������
		vector<string> da_way;

		//�������� ���������� �������
		visited.push_back(finish);

		//�����������, ���� �������� ��� �� ��� �������
		if (visited.size() < vertex_list.size())
		{
			for (int i = 0; i < vertex_list.size(); ++i)
			{
				//���� ������� ��� ����� ���������� � � ��� ����� ������, �� ������ �������� ��
				if (check_insertion(visited, vertex_list[i]) == false && get_weight(vertex_list[i], finish) > 0)
				{
					options.push_back(recursive_sailsman(visited, vertex_list[i]));
				}
			}

			//���� ���� ��������� �����, �� �������� ����������
			if (options.size() > 0)
			{
				for (int i = 0; i < options.size(); ++i)
				{
					if (options[i].size() > 0)
					{
						if (((get_route_distance(options[i]) + get_weight(options[i][0], finish)) < shortest) || shortest == -1)
						{
							shortest = get_route_distance(options[i]) + get_weight(options[i][0], finish);
							final_route = options[i];
						}
					}
				}

				//���� ����� �����-�� ����, �� ������� ��� � �������������
				if (final_route.size() > 0)
				{
					da_way.push_back(finish);

					for (int i = 0; i < final_route.size(); ++i)
					{
						da_way.push_back(final_route[i]);
					}
				}
			}

			return da_way;
		}
		//���� �������� ��� �������, �� ���������� �������� �������
		else
		{
			//���� �� �������� ����� ��� ������� ���� � ����� ������, �� ������ ������ ������
			if (get_weight(vertex_list[0], finish) > 0)
			{
				final_route.push_back(finish);
				final_route.push_back(vertex_list[0]);
			}

			return final_route;
		}
	}

	//����������� �������� � ������ ��������� �������
	void set_start(vector<string>& route, const string& start)
	{		
		if (route.size() > 0)
		{
			if (count(route.begin(), route.end(), start) > 0)
			{
				if (route[0] != start)
				{
					vector<string> ans;
					int start_i = 0;
					bool flag_break = false;
					for (int i = 0; i < route.size() - 1 && flag_break == false; ++i)
					{
						if (route[i] == start)
						{
							flag_break = true;
							start_i = i;
						}
					}
					for (int i = start_i; i < route.size() - 1; ++i)
					{
						ans.push_back(route[i]);
					}
					for (int i = 0; i < start_i; ++i)
					{
						ans.push_back(route[i]);
					}
					ans.push_back(start);
					route = ans;
				}
			}			
		}
	}

	//��������� �������� ������� ��������
	vector<string> build_route(const string& start)
	{
		vector<string> visited;
		vector<string> ans = recursive_sailsman(visited, vertex_list[0]);
		set_start(ans, start);
		cout << "---------------------------------------------------------" << endl;
		print(matrix, vertex_list);
		cout << endl;
		cout << "Route length: " << get_route_distance(ans) << endl;
		cout << "Our route: " << ans << endl;
		return ans;
	}

	//������� ������� � ������������� �������
	vector<string> delete_vertex(const string& vertex, vector<Coords>& coords, const string& start)
	{
		int i = get_index(vertex);
		vertex_list.erase(vertex_list.begin() + i);
		matrix.erase(matrix.begin() + i);
		for (int j = 0; j < matrix.size(); ++j)
		{
			matrix[j].erase(matrix[j].begin() + i);
		}
		coords.erase(coords.begin() + i);

		return build_route(start);
	}

	void insert_vertex(const string& vertex)
	{
		vertex_list.push_back(vertex);
	}	

	void insert_edge(const string& vrtx1, const string& vrtx2, const int& weight = 1)
	{
		int pos1 = get_index(vrtx1);
		int pos2 = get_index(vrtx2);

		if (pos1 == -1 || pos2 == -1)
		{
			cout << "��� ������� ���� �� ������ ����������� � �����" << endl;
		}
		else
		{
			if (matrix[pos1][pos2] == 0 and matrix[pos2][pos1] == 0)
			{
				matrix[pos1][pos2] = weight;
				matrix[pos2][pos1] = weight;
				matrix[pos1][pos1] = 0;
				matrix[pos2][pos2] = 0;
			}
			else
			{
				cout << "����� ��� ����������" << endl;
			}
		}
	}

	//�������������� ��������� ����� �� �������� 11 ������������ ������
	void autogen()
	{
		insert_vertex("Lisva");
		insert_vertex("Tinda");
		insert_vertex("Moscow");
		insert_vertex("Kazan");
		insert_vertex("Perm");
		insert_vertex("Ufa");
		insert_vertex("Orda");

		insert_edge("Moscow", "Ufa", 5);
		insert_edge("Ufa", "Orda", 19);
		insert_edge("Orda", "Tinda", 23);
		insert_edge("Tinda", "Kazan", 17);
		insert_edge("Lisva", "Kazan", 26);
		insert_edge("Perm", "Lisva", 9);
		insert_edge("Perm", "Moscow", 13);
		insert_edge("Perm", "Orda", 30);
		insert_edge("Ufa", "Kazan", 42);
	}	

	//���������� ����� ��������, ����������� � ������
	int get_route_distance(const vector<string>& route)
	{
		int dist = 0;
		for (int i = route.size() - 1; i > 0; --i)
		{
			dist += get_weight(route[i - 1], route[i]);
		}
		return dist;
	}	

	//���������� ����� ����� ���������� SFML
	void draw(vector<string>& route)
	{
		int width = 1000;
		int height = 950;
		RenderWindow MainWindow(VideoMode(width, height), "Our Graph", sf::Style::Titlebar | sf::Style::Close);

		//������� ������� �� ������ Next step
		int count = 0;

		//���������� ���� ������ �����
		vector<Coords> coords(size());
		coords[0].set(3 * D, 6 * D);
		coords[1].set(8 * D, 5 * D);
		coords[2].set(D, D);
		coords[3].set(6 * D, 8 * D);
		coords[4].set(2 * D, 3 * D);
		coords[5].set(4 * D, D);
		coords[6].set(7 * D, 2 * D);		

		//������, ���� �����-�� ������� ����� �������
		bool flag_move = false;		

		//������ �������, ������� ����� ��������
		int to_move_index;

		//������� ����� ������������ ������� � ������������ �������, ������� �� ��������
		int dx, dy;

		//��������� ����� ��������
		string start = "Tinda";

		while (MainWindow.isOpen())
		{
			Event event;

			while (MainWindow.pollEvent(event))
			{
				//������� �������� ����
				if (event.type == Event::Closed)
				{
					MainWindow.close();					
				}

				else
				//������� ������� �� �����-�� �� ������ ����
				if (event.type == Event::MouseButtonPressed)
				{
					//������� ������� �� ����� ������ ����
					if (event.mouseButton.button == Mouse::Left)
					{						
						//������� ������� �� ������ Next step
						if (Mouse::getPosition(MainWindow).x > 100 - btn_width / 2 &&
							Mouse::getPosition(MainWindow).x < 100 + btn_width / 2 &&
							Mouse::getPosition(MainWindow).y > 875 - btn_height / 2 &&
							Mouse::getPosition(MainWindow).y < 875 + btn_height / 2)
						{
							if (count < size())
							{
								++count;
							}
						}					
						
						else
						//������� ������� �� ������ Add Vertex
						if (Mouse::getPosition(MainWindow).x > 100 - btn_width / 2 &&
							Mouse::getPosition(MainWindow).x < 100 + btn_width / 2 &&
							Mouse::getPosition(MainWindow).y > 835 - btn_height / 2 &&
							Mouse::getPosition(MainWindow).y < 835 + btn_height / 2)
						{
							//������� ���������� ������������ ����� �������� (������ ��� ������ ����� ���������� ����� ����������)
							count = 0;
							
							cout << "������� �������� ������: ";
							string name1;
							cin >> name1;

							//������ �� ���������� ����� �������
							while (get_index(name1) != -1)
							{
								cout << "����� ��� ������������ � �����" << endl;
								cout << "������� �������� ������: ";								
								cin >> name1;
							}			
															
							insert_vertex(name1);									

							//������� ������ � �������
							for (int j = 0; j < vertex_list.size() - 1; ++j)
							{
								matrix[j].push_back(0);
							}

							//������� ������� � �������
							vector<int> additional_column(vertex_list.size());
							for (int j = 0; j < additional_column.size(); ++j)
							{
								additional_column[j] = 0;
							}
							matrix.push_back(additional_column);
								
							cout << "������� ���������� �������, ������� ��������� � ���� ��������: ";
							int count;
							cin >> count;

							//������ �� ����� ������������� ���������� �������
							while (count < 0)
							{
								cout << "���������� ������� �� ����� ���� ������������� ���������" << endl;
								cout << "������� ���������� �������, ������� ��������� � ���� ��������: ";
								cin >> count;
							}

							for (int i = 0; i < count; ++i)
							{									
								cout << "������� �������� ������: ";
								string name2;
								cin >> name2;

								//������ �� ����� ��������������� ������
								while (get_index(name2) == -1)
								{
									cout << "����� ����������� � �����" << endl;
									cout << "������� �������� ������: ";									
									cin >> name2;
								}

								//������ �� ������� ��������� �����
								while (get_weight(name1, name2) > 0)
								{
									cout << "����� ����� �������� ��� ����������" << endl;
									cout << "������� �������� ������: ";									
									cin >> name2;
								}

								cout << "������� ����������: ";
								int dist;
								cin >> dist;

								//������ �� ����� ���������������� ����������
								while (dist <= 0)
								{
									cout << "���������� ������ ���� ������������� ���������" << endl;
									cout << "������� ����������: ";									
									cin >> name2;
								}

								insert_edge(name1, name2, dist);
							}						

							//���������� ������� � ������� ��� �����						
							route = build_route(start);

							//������� ����� ����� � ����� ������, ����� ������������ ��� ��� ��������� ���
							Coords new_one;
							new_one.set(width / 2, height / 2);
							coords.push_back(new_one);
							
						}						

						//������� �������������� �������
						else
						{
							for (int i = 0; i < size(); ++i)
							{
								if (Mouse::getPosition(MainWindow).x > coords[i].x - R &&
									Mouse::getPosition(MainWindow).x < coords[i].x + R &&
									Mouse::getPosition(MainWindow).y > coords[i].y - R &&
									Mouse::getPosition(MainWindow).y < coords[i].y + R)
								{
									//����������, ����� ������� ����� �������
									to_move_index = i;

									//���������� ���������� ������� ������������ �������
									dx = event.mouseButton.x - coords[i].x;
									dy = event.mouseButton.y - coords[i].y;

									//����������, ��� �����-�� �� ������ ����� ��������� �� ��������
									flag_move = true;
								}
							}
						}						
					}

					else
					//������� ������� �� ������ ������ ���� ��� �������� �������
					if (event.mouseButton.button == Mouse::Right)
					{
						bool flag_break = false;
						for (int i = 0; i < size() && flag_break == false; ++i)
						{
							if (Mouse::getPosition(MainWindow).x > coords[i].x - R &&
								Mouse::getPosition(MainWindow).x < coords[i].x + R &&
								Mouse::getPosition(MainWindow).y > coords[i].y - R &&
								Mouse::getPosition(MainWindow).y < coords[i].y + R)
							{		
								//����� ���������� ������������ ������� ��������, ���� ������� ����� ����������
								count = 0;

								if (start == vertex_list[i])
								{
									start = vertex_list[0];
								}
								route = delete_vertex(vertex_list[i], coords, start);								
								flag_break = true;
							}
						}
					}

					else
					//������� ������� �� ������� ������ ��� ������� ��������� �����
					if (event.mouseButton.button == Mouse::Middle)
					{
						bool flag_break = false;
						for (int i = 0; i < size() && flag_break == false; ++i)
						{
							if (Mouse::getPosition(MainWindow).x > coords[i].x - R &&
								Mouse::getPosition(MainWindow).x < coords[i].x + R &&
								Mouse::getPosition(MainWindow).y > coords[i].y - R &&
								Mouse::getPosition(MainWindow).y < coords[i].y + R)
							{
								//����� ���������� ������������ ������� ��������, ���� ������� ����� ����������
								count = 0;
								
								start = vertex_list[i];
								route = build_route(start);
								flag_break = true;
							}
						}
					}
				}

				else
				//������� ���������� ����� ������ ����
				if (event.type == Event::MouseButtonReleased)
				{
					if (event.key.code == Mouse::Left)
					{
						flag_move = false;
					}					
				}				
			}

			MainWindow.clear(Color::White);

			//���� ������ ����� ������ ����, �� ������� ���������� �������, �� ������� ��������� ������ (���� ����� ����)
			if (flag_move == true)
			{
				//�������������� �������� ������ �� �������
				if (((Mouse::getPosition(MainWindow).x - dx) > R) && ((Mouse::getPosition(MainWindow).x - dx) < (width - R)))
				{
					coords[to_move_index].set(Mouse::getPosition(MainWindow).x - dx, coords[to_move_index].y);
				}

				//������������ �������� ������ �� �������
				if (((Mouse::getPosition(MainWindow).y - dy) > R) && ((Mouse::getPosition(MainWindow).y - dy) < (height - R)))
				{
					coords[to_move_index].set(coords[to_move_index].x, Mouse::getPosition(MainWindow).y - dy);
				}
				
			}

			//��������� �����, ����������� �������, � ����� ���� ����� ����� ����
			for (int i = 0; i < size(); ++i)
			{
				vector<string> neighbours = get_neighbour_list(vertex_list[i]);
				for (int j = 0; j < neighbours.size(); ++j)
				{
					stringstream txt;
					txt << matrix[i][get_index(neighbours[j])];
					draw_line(coords[i], coords[get_index(neighbours[j])], txt.str(), MainWindow);
				}
			}

			//��������� ���� ������ ������� ������
			for (int i = 0; i < size(); ++i)
			{
				stringstream i_txt;
				i_txt << vertex_list[i];
				draw_vertex(coords[i], i_txt.str(), false, MainWindow);
			}

			//������ �������� count ������� ������ - ������ count ��������� ������ ��������
			for (int i = 0; i < count; ++i)
			{
				if (route.size() >= count)
				{
					draw_vertex(coords[get_index(route[i])], route[i], true, MainWindow);
				}
			}

			//���������� ������ � �� �����
			Coords button_coords;
			string text;

			//��������� ������ Next step			
			button_coords.set(100, 875);
			text = "Next Step";
			draw_button(button_coords, btn_width, btn_height, text, MainWindow);

			//��������� ������ Add Vertex			
			button_coords.set(100, 835);
			text = "Add Vertex";
			draw_button(button_coords, btn_width, btn_height, text, MainWindow);

			//���������� ������
			Coords text_coords;
			
			//��������� �� ���������� ��������������� ������
			text_coords.set(40, 680);
			text = "Left Mouse Button to move vertex";
			draw_message(text_coords, text, false, MainWindow);

			//��������� �� ���������� ���������
			text_coords.set(40, 700);
			text = "Right Mouse Button to delete vertex";
			draw_message(text_coords, text, false, MainWindow);

			//��������� �� ���������� ������ ��������� �������
			text_coords.set(40, 720);
			text = "Middle Mouse Button to set starting vertex";
			draw_message(text_coords, text, false, MainWindow);

			//��������� � ������� ��������� �������
			text_coords.set(40, 740);
			text = "Current starting vertex: " + start;
			draw_message(text_coords, text, false, MainWindow);

			//���������� ��� ������ ������, ����������� ���������� � ��������
			stringstream route_txt;

			//��������� � ������� ��������
			text_coords.set(40, 760);
			route_txt << "Current route: " << route;			
			draw_message(text_coords, route_txt.str(), false, MainWindow);

			//��������� � ������� ����� ��������
			text_coords.set(40, 780);
			route_txt.str("");
			route_txt << "Current route length: " << get_route_distance(route);
			draw_message(text_coords, route_txt.str(), false, MainWindow);

			MainWindow.display();
		}
	}
};

int main()
{
	setlocale(LC_ALL, "RUS");

	//�������� ����� �� �������� 11 � ����� ��� �������
	Graph example(N);
	example.autogen();	

	//��������� � ����� �������� � ��� �����	
	vector<string> route = example.build_route("Tinda");	

	system("pause");

	//���������� ������������� ����� � ��������
	example.draw(route);	

	return 0;
}