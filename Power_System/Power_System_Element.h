#ifndef OPENCL_POWERFLOW_POWER_SYSTEM_ELEMENT_H
#define OPENCL_POWERFLOW_POWER_SYSTEM_ELEMENT_H

#include <vector>

namespace opps {
	template<typename ElementType>
	class SystemElement {
	private:

		class RowData {
		public:
			explicit RowData(const std::vector<double> &data);

			double Get(ElementType idx);
			void Set(ElementType idx, double val);

			std::vector<double> GetData();
			void SetData(std::vector<double> &val);

		private:
			std::vector<double> line;
		};

		std::vector<RowData> internalData;

	public:
		int nrOfRows;
		int nrOfCols;

		explicit SystemElement(const std::vector<std::vector<double> > &input);

		std::vector<double> GetRow(int row);
		void SetRow(int idx, std::vector<double> &row);

		std::vector<double> GetCol(ElementType col);
		void SetCol(ElementType col, std::vector<double> &data);

		double GetVal(int row, ElementType col);
		void SetVal(int row, ElementType col, double val);

		int RowSize();
		int ColSize();

	};
}
#endif //OPENCL_POWERFLOW_POWER_SYSTEM_ELEMENT_H
