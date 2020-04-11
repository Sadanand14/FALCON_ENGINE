#ifndef LINE_H
#define LINE_H

#include <framework.h>
#include <string>
#include <System/Types.h>

#include <RenderPasses/Renderable.h>

enum class LineType { LINE, LOOP, STRIP };

/**
 * Stores data for lines
 */
class Line : public Renderable
{
private:
	//Render Data
	VertexBuffer* m_VBO;
	boost::container::vector<glm::vec3, fmemory::STLAllocator<glm::vec3>> m_points;
	glm::vec3 m_color;
	LineType m_type = LineType::LINE;

public:
	Line();
	virtual ~Line();

	//Functions
	void Setup() override;
	void Bind() override;

	inline void AddPoint(const glm::vec3 &point) { m_points.push_back(point); }
	inline void ClearPoints() { m_points.clear(); }
	inline void SetLineType(LineType type) { m_type = type; }
	inline LineType GetLineType() { return m_type; }
	inline void SetColor(glm::vec3 color) { m_color = color; }
	inline glm::vec3 GetColor() const{ return m_color; }
	inline u32 GetPointAmount() { return m_points.size(); }
};

#endif //!MESH_H
