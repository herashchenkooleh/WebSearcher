import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import com.data_item 1.0

RowLayout {
    spacing: 5
    Layout.preferredHeight: 40
    Layout.fillHeight: false
	Layout.fillWidth: true
    property var d

	Text {
		id: url
		Layout.fillWidth: true
        text: modelData.url
        width: 200
        padding: 10
    }

    Text {
		id: status
        text: "Status: " + modelData.status
        width: 250
        padding: 10
    }

	Text {
		id: errorCode
        text: "Error: " + modelData.errorCode
        width: 250
        padding: 10
    }
}