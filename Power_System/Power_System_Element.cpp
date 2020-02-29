#include "Power_System_Element.h"

using namespace opps;

template<typename ElementType>
SystemElement<ElementType>::SystemElement(const std::vector<std::vector<double> > &input) {
	nrOfRows = 0;
	nrOfCols = 0;
	for (const auto &row : input) {
		internalData.emplace_back(RowData(row));
		nrOfRows++;
	}
	for (const auto col : input[0]) {
		nrOfCols++;
	}
}

template<typename ElementType>
std::vector<double> SystemElement<ElementType>::GetRow(int row) {
	return internalData[row].GetData();
}

template<typename ElementType>
void SystemElement<ElementType>::SetRow(int idx, std::vector<double> &row) {
	internalData[idx].SetData(row);
}

template<typename ElementType>
double SystemElement<ElementType>::GetVal(int row, ElementType col) {
	return internalData[row].Get(col);
}

template<typename ElementType>
void SystemElement<ElementType>::SetVal(int row, ElementType col, double val) {
	internalData[row].Set(col, val);
}

template<typename ElementType>
int SystemElement<ElementType>::RowSize() {
	return nrOfRows;
}

template<typename ElementType>
int SystemElement<ElementType>::ColSize() {
	return nrOfCols;
}

template<typename ElementType>
std::vector<double> SystemElement<ElementType>::GetCol(ElementType col) {

	std::vector<double> retCol;

	for (const RowData &i : internalData) {
		retCol.emplace_back(i.Get(col));
	}

	return retCol;
}

template<typename ElementType>
void SystemElement<ElementType>::SetCol(ElementType col, std::vector<double> &data) {
	int count = { 0 };
	if (data.size() != nrOfRows) {
		//size mismatch, do not modify
		return;
	}
	for (RowData &i : internalData) {
		i.Set(col, data[count]);
		count++;
	}

}


/**private<**/
template<typename ElementType>
SystemElement<ElementType>::RowData::RowData(const std::vector<double> &data) {
	for (const auto &i : data) {
		line.push_back(i);
	}
}

template<typename ElementType>
double SystemElement<ElementType>::RowData::Get(const ElementType idx) {
	return line[(int)idx];
}

template<typename ElementType>
void SystemElement<ElementType>::RowData::Set(const ElementType idx, const double val) {
	line[(int)idx] = val;
}

template<typename ElementType>
std::vector<double> SystemElement<ElementType>::RowData::GetData() {
	return line;
}

template<typename ElementType>
void SystemElement<ElementType>::RowData::SetData(std::vector<double> &val) {
	line = val;
}
/**>private**/