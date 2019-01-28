import QtQuick 2.12
import QtWebView 1.1
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import com.qml_cpp_bridge 1.0

ApplicationWindow {
    width: 1280
    height: 720
    visible: true

	QmlCppBridge {
		id : bridge
	}

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
		anchors.margins: 10, 10, 10, 10

        RowLayout {
            spacing: 5
            Layout.preferredHeight: 40
            Layout.fillHeight: false
			Layout.fillWidth: true

			Text {
				Layout.fillHeight: false
				Layout.fillWidth: false
				text: "url"
			}

			TextField {
				id: url
				Layout.fillHeight: false
				Layout.fillWidth: true
				text: "http://"
			}
        }

		RowLayout {
            spacing: 5
            Layout.preferredHeight: 40
            Layout.fillHeight: false
			Layout.fillWidth: true

			Text {
				Layout.fillHeight: false
				Layout.fillWidth: false
				text: "search text"
			}

			TextField {
				id: searchText
				Layout.fillHeight: false
				Layout.preferredWidth: 550
				Layout.fillWidth: true
				text: ""
			}

			Text {
				Layout.fillHeight: false
				Layout.fillWidth: false
				text: "depth of search"
			}

			TextField {
				id: depthOfSearch
				Layout.fillHeight: false
				Layout.preferredWidth: 150
				Layout.fillWidth: false
				text: ""
			}

			Text {
				Layout.fillHeight: false
				Layout.fillWidth: false
				text: "count threads"
			}

			TextField {
				id: countThreads
				Layout.fillHeight: false
				Layout.preferredWidth: 150
				Layout.fillWidth: false
				text: ""
			}

			Button {
				text: "search"
				onClicked: bridge.startSearch(
					url.text,
					searchText.text,
					depthOfSearch.text,
					countThreads.text
				)
			}
        }

		ColumnLayout {
			id: layout
			spacing: 5
			Layout.preferredHeight: 40
			Layout.fillHeight: true
			Layout.fillWidth: true

			ListView {
				id: listView
				Layout.fillHeight: true
				Layout.fillWidth: true

				model: searchItemList

				delegate: DataItemDelegate {
					d: searchItemList.item(index)
					anchors.left: parent.left
					anchors.right: parent.right
				}
			}
		}

		ProgressBar {
			Layout.preferredHeight: 30
			Layout.fillHeight: false
			Layout.fillWidth: true
			id: progressBar
			from: 0
			to: Count
			value: 0

			Connections {
				target: worker
				onProgressChanged: {
					progressBar.value = progress;
				}
			}
		}
    }
}